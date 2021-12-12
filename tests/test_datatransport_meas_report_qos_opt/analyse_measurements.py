
from tkinter import filedialog
from tkinter import *
import csv
import numpy as np
import pandas as pd
import seaborn as sns
import matplotlib.pyplot as plt
from enum import IntEnum
import os
import hashlib

########################################################################################################################
# Defines
########################################################################################################################

FILE_SIZE = 5242880  # Discharge all files above 5242880 bytes this means that an error would have happened


class FileSize(IntEnum):
    Fine = 0
    ToBig = 1
    ToSmall = 2


########################################################################################################################
# Class define, class used to store analyzed data
########################################################################################################################


class FileTestParameters:
    def __init__(self):
        self.send_delay = None
        self.block_size = None
        self.file_size = None
        self.bytes_dropped = np.nan
        self.full_hash = None
        self.hashes = []
        self.correct_full_hash = None
        self.correct_hashes = []
        self.correct_order = []
        self.number_of_packets_out_of_order = 0
        self.number_of_packets = 0

    def __str__(self):
        return "\nSend_delay: {}".format(self.send_delay) + ", Block_size: {}".format(self.block_size) + \
               "\nFile_size: {}".format(self.file_size) + ", Bytes_dropped: {}".format(self.bytes_dropped) + \
               "\nNumber of packets: {}".format(self.number_of_packets) + \
               ", Packets out of order: {}".format(self.number_of_packets_out_of_order)


class GatheredTestParameters:
    def __init__(self):
        self.files = []
        self.n_to_big = 0
        self.n_to_small = 0
        self.n_fine_size = 0
        self.throughput = []

    def __str__(self):
        print_str = ""
        print_str += "Files fine size: {}, files to big: {}, files to small: {}, \n".format(self.n_fine_size,
                                                                                            self.n_to_big,
                                                                                            self.n_to_small)
        for file in self.files:
            print_str += file.__str__()
            print_str += "\n"
        return print_str


########################################################################################################################
# Global vars
########################################################################################################################


block_size = [4096, 2048, 1024, 512, 256, 128, 64]
send_delay = [150, 130, 100, 70, 50, 20]
analyzed_data = []

hash_64 = FileTestParameters()
hash_128 = FileTestParameters()
hash_256 = FileTestParameters()
hash_512 = FileTestParameters()
hash_1024 = FileTestParameters()
hash_2048 = FileTestParameters()
hash_4096 = FileTestParameters()
test_hashes = {64: hash_64, 128: hash_128,  256: hash_256, 512: hash_512, 1024: hash_1024, 2048: hash_2048,
               4096: hash_4096}


def hash_file(file, file_parameters):
    """"
    This function returns the SHA-1 hash
    of the file passed into it
    @note Stolen from: https://www.programiz.com/python-programming/examples/hash-file
    """

    # make a hash object
    h = hashlib.sha1()

    # loop till the end of the file
    chunk = 0
    while chunk != b'':
        # make a hash object
        h_1 = hashlib.sha1()

        # read only 1024 bytes at a time
        chunk = file.read(5242)

        h.update(chunk)
        h_1.update(chunk)
        file_parameters.hashes.append(h_1.hexdigest())

    file_parameters.full_hash = h.hexdigest()
    # return the hex representation of digest
    return h.hexdigest()


def compare_hashes(file_parameters):
    if file_parameters.file_size != FILE_SIZE:
        file_parameters.correct_full_hash = False
    else:
        file_parameters.correct_full_hash = True
    for tested_hash, org_hash in zip(file_parameters.hashes, test_hashes[file_parameters.block_size].hashes):
        if tested_hash != org_hash:
            file_parameters.correct_hashes.append(False)
        else:
            file_parameters.correct_hashes.append(True)


def get_initial_file_parameters(test_parameters, file_name):
    for block in block_size:
        location = file_name.find(str(block))
        if location > 0:
            test_parameters.block_size = block
            break
    for delay in send_delay:
        location = file_name.find(str(delay))
        if location > 0:
            test_parameters.send_delay = delay
            break


def check_file_size(test_parameters, file):
    file_size = os.path.getsize(file)
    test_parameters.file_size = file_size
    if file_size < FILE_SIZE:
        test_parameters.bytes_dropped = FILE_SIZE - file_size
        return FileSize.ToSmall
    elif file_size > FILE_SIZE:
        return FileSize.ToBig
    else:
        test_parameters.bytes_dropped = 0
        return FileSize.Fine


def analyze_corruptness(file_parameters, file_path):
    """
    Analyzes the corruptness of the file
    :param file_parameters: FileParameters object
    :param file_path: File path of the file to analyze
    :return: None
    """
    f = open(file_path, "rb")
    hash_file(f, file_parameters)
    f.close()
    compare_hashes(file_parameters)


def analyze_package_order(file_parameters, file_path):
    f = open(file_path, "rb")

    # loop till the end of the file
    chunk = 0
    i = 0
    while chunk != b'':
        file_parameters.number_of_packets += 1

        # read only 1024 bytes at a time
        chunk = f.read(file_parameters.block_size)

        packet_number = int.from_bytes(chunk[:5], "big")
        if packet_number == i:
            file_parameters.correct_order.append(True)
        else:
            file_parameters.correct_order.append(False)
            file_parameters.number_of_packets_out_of_order += 1
        i += 1
    f.close()


def calculate_throughput(path_to_file, parameters):
    elements = None
    with open(path_to_file + "/measured_qos_opt_data.csv") as csv_file:
        csv_obj = csv.reader(csv_file, delimiter=',')
        for row in csv_obj:
            elements = row
            break

    i = 0
    for delay in send_delay:
        for block in block_size:
            try:
                trans = ((FILE_SIZE*8) / 10**6) / (int(elements[i])/1000000)  # Transmission rate [Bytes/s]
                parameters.throughput.append([delay, block, trans])
            except ValueError:
                parameters.throughput.append([delay, block, 0])
            i += 1


def analyze_file(file_path, file_name):
    """
    In charge of Getting different parts og the file analyzed
    :param file_path:
    :param file_name:
    :return:
    """
    file_parameters = FileTestParameters()
    full_path = file_path + file_name

    get_initial_file_parameters(file_parameters, file_name)

    file_flag = check_file_size(file_parameters, full_path)
    if file_flag == FileSize.ToBig:
        # If the file size is to big or to small, there is no reason to continue analyzing
        file_parameters.number_of_packets_out_of_order = "N/a"
        file_parameters.number_of_packets = "N/a"
        return file_parameters, file_flag

    analyze_corruptness(file_parameters, full_path)
    analyze_package_order(file_parameters, full_path)

    return file_parameters, file_flag


def analyze_files(path_to_files):
    """
    Finds all files one at the time, and then sends them to be analyzed
    :param path_to_files: The path that contains all the slave directories
    :return: parameters
    """
    parameters = GatheredTestParameters()
    file_path = path_to_files+"/s1/rcv_file_64_50.bin"

    for slave in os.listdir(path_to_files):
        try:
            for file in os.listdir(path_to_files + "/" + slave):
                file_parameters, size_flag = analyze_file(path_to_files + "/" + slave + "/", file)
                parameters.files.append(file_parameters)
                if size_flag == FileSize.Fine:
                    parameters.n_fine_size += 1
                elif size_flag == FileSize.ToBig:
                    parameters.n_to_big += 1
                elif size_flag == FileSize.ToSmall:
                    parameters.n_to_small += 1
        except NotADirectoryError:
            continue

    calculate_throughput(path_to_files, parameters)
    return parameters


def plot_throughput(in_data):

    in_data = np.delete(in_data, [-1, -2, -3]).reshape(41, 3)
    data_frame = pd.DataFrame(in_data, columns=["send_delay", "block_size", "time_used"])
    time_used = (np.asarray(data_frame['time_used']))
    result = data_frame.pivot(index='send_delay', columns='block_size', values='time_used')

    plt.style.use("seaborn")
    # 3. Plot the heatmap
    plt.figure(figsize=(12, 7))
    heat_map = sns.heatmap(result, linewidth=1, annot=True, fmt='.2f', cmap="YlGnBu",
                           cbar_kws={'label': 'Throughput [MB/s]'})
    # plt.show()
    plt.savefig('diagrams/throughput.png')


def plot_byte_loss(in_data):
    tmp_data = in_data
    raw_list = []
    for file in tmp_data.files:
        # 'N/a' are counted as dropped the whole file
        if file.bytes_dropped is np.nan:
            file.bytes_dropped = FILE_SIZE
        raw_list.append([file.send_delay, file.block_size, file.bytes_dropped])

    # Create dataframe from raw_list
    df = pd.DataFrame(raw_list, columns=["send_delay", "block_size", "bytes_dropped"])

    # Sum all dropped bytes.
    summed_list = df.groupby(['send_delay', 'block_size']).sum().reset_index()

    # Create pivot
    result = summed_list.pivot(index='send_delay', columns='block_size', values='bytes_dropped')

    # Plot
    plt.style.use("seaborn")
    # 3. Plot the heatmap
    plt.figure(figsize=(12, 7))
    ax = sns.heatmap(result, linewidth=1, annot=True, fmt='.0f', cmap="YlGnBu",
                     cbar_kws={'label': 'Bytes dropped'})
    ax.set_xlabel('block_size [Bytes]')
    ax.set_ylabel('send_delay [us]')
    # plt.show()
    plt.savefig('diagrams/byte_loss.png')


def plot_file_corruption(in_data):
    tmp_data = in_data
    raw_list = []
    for file in tmp_data.files:
        # 'N/a' are counted as dropped the whole file
        raw_list.append([file.send_delay, file.block_size, file.correct_hashes])

    counted_list = []
    for element in raw_list:
        length = element[2].__len__()
        if length == 0:
            permil = 0
        else:
            n_true = sum(element[2])
            permil = 1000 - round(n_true/length * 1000)
        counted_list.append([element[0], element[1], permil])

    # Create dataframe from raw_list
    df = pd.DataFrame(counted_list, columns=["send_delay", "block_size", "permil"])

    # Sum all dropped bytes.
    summed_list = df.groupby(['send_delay', 'block_size']).sum().reset_index()

    # normalize values after sum
    summed_list['permil'] = summed_list['permil'] / 8

    # Create pivot
    result = summed_list.pivot(index='send_delay', columns='block_size', values='permil')

    # Plot
    plt.style.use("seaborn")
    # 3. Plot the heatmap
    plt.figure(figsize=(12, 7))
    ax = sns.heatmap(result, linewidth=1, annot=True, fmt='.0f', cmap="YlGnBu",
                     cbar_kws={'label': 'Corruptness in permil'})
    ax.set_xlabel('block_size [Bytes]')
    ax.set_ylabel('send_delay [us]')
    # plt.show()
    plt.savefig('diagrams/file_corruptness.png')


def plot_package_order_correctness(in_data):
    tmp_data = in_data
    raw_list = []
    for file in tmp_data.files:
        # 'N/a' are counted as dropped the whole file
        raw_list.append([file.send_delay, file.block_size, file.correct_order])

    counted_list = []
    for element in raw_list:
        length = element[2].__len__()
        if length == 0:
            permil = 0
        else:
            n_true = sum(element[2])
            permil = round(n_true/length * 1000)
        counted_list.append([element[0], element[1], permil])

    # Create dataframe from raw_list
    df = pd.DataFrame(counted_list, columns=["send_delay", "block_size", "permil"])

    # Sum all dropped bytes.
    summed_list = df.groupby(['send_delay', 'block_size']).sum().reset_index()

    # normalize values after sum
    summed_list['permil'] = summed_list['permil'] / 8

    # Create pivot
    result = summed_list.pivot(index='send_delay', columns='block_size', values='permil')

    # Plot
    plt.style.use("seaborn")
    # 3. Plot the heatmap
    plt.figure(figsize=(12, 7))
    ax = sns.heatmap(result, linewidth=1, annot=True, fmt='.0f', cmap="YlGnBu",
                     cbar_kws={'label': 'Correct order in permil'})
    ax.set_xlabel('block_size [Bytes]')
    ax.set_ylabel('send_delay [us]')
    # plt.show()
    plt.savefig('diagrams/byte_order.png')


def analyze_data(data):
    plot_throughput(np.array(data.throughput, dtype=float))
    plot_byte_loss(data)
    plot_file_corruption(data)
    plot_package_order_correctness(data)


def get_path_to_files():
    root = Tk()
    root.withdraw()
    path = filedialog.askdirectory()
    return path


def hash_test_files():
    for file, hash_par in zip(test_hashes.keys(), test_hashes.values()):
        f = open("test_files/send_file_{}.bin".format(file), "rb")
        hash_file(f, hash_par)
        hash_par.block_size = file
        hash_par.file_size = FILE_SIZE
        f.close()


def main():
    hash_test_files()
    path_to_files = get_path_to_files()
    data = analyze_files(path_to_files)
    analyze_data(data)
    pass


if __name__ == "__main__":
    main()

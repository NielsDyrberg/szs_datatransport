"""
This script will analyze test of QoS on datatransport


"""
import random
import os
import sys
import csv
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

NUMBER_OF_SLAVES = 8


def plot_data(data):

    plt.rc('axes', labelsize=20)
    # make dataframe
    df = pd.DataFrame(data, columns=["1", "2", "3", "4", "5", "6", "7", "8"])
    ax = df.boxplot(column=["1", "2", "3", "4", "5", "6", "7", "8"], figsize=(500, 5), grid=False, fontsize=20, rot=0,
                    showfliers=False)

    # plot
    # ax.set_title('Boxplot of round-trip time [us]')
    ax.set_xlabel('Pi\'s #')
    ax.set_ylabel('Round-trip time [us]')

    plt.show()


def do_statistics(data):
    print("Min: {}".format(data.min()))
    print("Max: {}".format(data.max()))
    print("Mean: {}".format(data.mean()))
    print("Jitter: {}".format(data.max()-data.min()))


def retrieve_measured_data():
    data = np.genfromtxt("measured_data/measured_data.csv", delimiter=',', skip_header=1)  # It is not skipping a header
    # but the first line of data, as the first ping is very slow.
    return data


def main():
    in_data = retrieve_measured_data()
    do_statistics(in_data)
    plot_data(in_data)
    pass


if __name__ == "__main__":
    main()

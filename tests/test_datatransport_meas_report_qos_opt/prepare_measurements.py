"""
This script will prepare test for optimization of QoS on datatransport


"""
import random
import os
import sys

from utils import ROOT_DIR
import paramiko
import subprocess
from threading import Thread

block_size = {"4096", "2048", "1024", "512", "256", "128", "64"}
hosts = ["192.168.1.47", "192.168.1.46"]

root_scr_path = "/tmp/szs_datatransport"
scr_path = "/tmp/szs_datatransport/tests/test_datatransport_meas_report_qos_opt"
test_files_path = "/tmp/szs_datatransport/tests/test_datatransport_meas_report_qos_opt/test_files"


def execute_tests():
    master_host = hosts[-1]
    ssh = []
    transport = []
    channel = []
    i = 0
    for host in hosts:
        ssh.append(paramiko.SSHClient())
        ssh[i].load_host_keys(os.path.expanduser(os.path.join("~", ".ssh", "known_hosts")))
        ssh[i].connect(host, username="ncpd", password="fantacola")
        transport.append(ssh[i].get_transport())
        channel.append(transport[i].open_session())

        if host == master_host:
            stdin, stdout, stderr = ssh[i].exec_command("cd " + scr_path + " ; nohup " +
                                                        "./test_datatransport_qos_opt_master > /dev/null 2>&1 &")
            print("stderr: ", stderr.readlines())
            print("pwd: ", stdout.readlines())
        else:
            channel[i].exec_command("cd " + scr_path + " ; nohup " +
                                    "./test_datatransports_qos_opt_slave > /dev/null 2>&1 &")
        i += 1


def build_projects():
    for host in hosts:
        ssh = paramiko.SSHClient()
        ssh.load_host_keys(os.path.expanduser(os.path.join("~", ".ssh", "known_hosts")))
        ssh.connect(host, username="ncpd", password="fantacola")
        print("---")
        print("cmake")
        print("---")
        stdin, stdout, stderr = ssh.exec_command("cd " + root_scr_path + " ; cmake .")
        print("stderr: ", stderr.readlines())
        print("pwd: ", stdout.readlines())

        print("---")
        print("make")
        print("---")
        stdin, stdout, stderr = ssh.exec_command("cd " + root_scr_path + " ; make")
        print("stderr: ", stderr.readlines())
        print("pwd: ", stdout.readlines())

        ssh.close()


def upload_files():
    for host in hosts:
        subprocess.run("scp -r " + ROOT_DIR + " ncpd@" + host + ":/tmp/")


def create_test_files():
    for block in block_size:
        sequence_number = 0
        f = open("test_files/send_file_" + block + ".bin", "wb")

        f.write(sequence_number.to_bytes(length=5, byteorder="big"))
        sequence_number += 1

        i = 5
        while i < 5242880:
            if (not i % int(block)) and (i >= int(block)):
                f.write(sequence_number.to_bytes(length=5, byteorder="big"))
                i += 5
                sequence_number += 1
            else:
                f.write(random.randint(0, 255).to_bytes(length=1, byteorder="big"))
                i += 1

        f.close()


def main():
    # create_test_files()
    upload_files()
    build_projects()
    # execute_tests()


if __name__ == "__main__":
    main()

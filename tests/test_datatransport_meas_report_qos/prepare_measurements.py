"""
This script will prepare test of QoS on datatransport


"""
import random
import os
import sys

from utils import ROOT_DIR
import paramiko
import subprocess
hosts = ["192.168.1.47", "192.168.1.46"]

root_scr_path = "/tmp/szs_datatransport"
scr_path = "/tmp/szs_datatransport/tests/test_datatransport_meas_report_qos_opt"
test_files_path = "/tmp/szs_datatransport/tests/test_datatransport_meas_report_qos_opt/test_files"


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


def main():
    upload_files()
    build_projects()


if __name__ == "__main__":
    main()

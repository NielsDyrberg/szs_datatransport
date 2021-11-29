"""
This script will analyze test of QoS on datatransport


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


def download_files():
    for host in hosts:
        subprocess.run("scp -r ncpd@" + host + ":/tmp/ " + ROOT_DIR )


def main():
    upload_files()


if __name__ == "__main__":
    main()

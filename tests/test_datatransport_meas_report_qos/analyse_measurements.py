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

    fig, ax = plt.subplots()

    # make dataframe
    df = pd.DataFrame(data, columns=["1", "2", "3", "4", "5", "6", "7", "8"])
    ax = df.boxplot(column=["1", "2", "3", "4", "5", "6", "7", "8"], figsize=(500, 5), grid=False, fontsize=15, rot=45,
                    showfliers=False)

    # plot
    ax.set_title('Boxplot of round-trip time [us]')
    ax.set_xlabel('Pi\'s #')
    ax.set_ylabel('Round-trip time')
    plt.show()


def do_statistics(data):
    min_delays = []
    max_delays = []
    mean_delays = []

    for i in range(0, NUMBER_OF_SLAVES):
        min_delays.append(data[:, i].min())
        max_delays.append(data[:, i].max())
        mean_delays.append(data[:, i].mean())

    print(min_delays, max_delays, mean_delays)
    plot_data(data)
    return None


def retrieve_measured_data():
    data = np.genfromtxt("measured_data/measured_data.csv", delimiter=',', skip_header=1)  # It is not skipping a header
    # but the first line of data, as the first ping is very slow.
    return data


def main():
    in_data = retrieve_measured_data()
    do_statistics(in_data)
    pass


if __name__ == "__main__":
    main()

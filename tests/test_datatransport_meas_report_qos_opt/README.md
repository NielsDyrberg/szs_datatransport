<!-- 
To compile puml use: (Assuming plantuml you are in the directory)
plantuml.jar -tsvg README.md -o diagrams
-->

# Optimization on the Datatransport package

This test will measure the effect of two test parameters on some some key values. These key values can be read under 
[Measurement values](#measurement-values). These values will be measured be sending a file from master to the slaves.
Some of the values will be used in the firmware, and some measurements will be used in order to check that it is possible to stay within spec. 

The measurement values will be found by analyzing the file that was received on the slave under different test scenarios, more on how the measurement values are found can be read in: [How will the measurement values be found](#how-will-the-measurement-values-be-found). 

The different test scenarios comes from adjusting the two test parameters (send_delay & block_size) which can be found under [Test parameters](#test-parameters).

## Measurement values

| Measurement | Description |
|---|---|
| Byte loss | How many bytes are lost under transmission |
| File corruptness | How much of the file has been corrupted under transmission |
| Packet out of order | How many packets arrive out of order |
| Throughput | The time it takes to send all the data to all slaves |

## Test parameters

Below the test parameters that has been found relevant will be listed. 
These parameters will be adjusted different tests, in order to see their effect on the measurements.

| Test parameter | Abbreviation | Description |
|---|---|---|
| Send delay | send_delay | How much time passes between one transition has ended until the next begins |
| Block size | block_size | The size of the file block that is being transmitted. |

## How will the measurement values be found.

This section will try to explain how the measurement values are found/calculated

### Byte loss 

By knowing the size of the file sent and the file received the byte loss will be calculated from `size(file_sent) - size(file_received)` 

### File corruptness

#### Complete file hash

In order to determine the file corruptness the sent file will hashed and the compared with the hash of the received file.

In semi pseudocode (leaning on python):
```{python}
assert( hash(file_sent), hash(file_received)  )
```

If the two hashes are equal it will be considered as the files are identical.

#### Hash blocks of the file

If the two hashes are not the same a more through hash check will be made. Here the two files will be divided into 1000 sub blocks that will be hashed and compared so it can be determined down to 0.1% how much of the file is corrupt.

In semi pseudocode (leaning on python):
```{python}
for thousandth_sent, thousandth_received in (file_send[::1000], file_received[::1000]):
    assert( hash(thousandth_sent), hash(thousandth_received) )
```

### Packet out of order

In order to detect if a packets are received in order, the file that is sent from master to slave has to be tailored to fit to **block_size**. This is done as UDP does not implement any kind of order detection/correction.

By tailoring a file to the block_size a file can be made 5 MiB big where each block_size'th element is equal to the block_number, this can then be analysed afterwards in order to determine the order of the received blocks.
The bytes that are undefined will just be randomly generated.

The file will have structure as illustrated bellow.
```
      -----------------------------------------------------
Byte # |  0x00  | ... | block_size | ... | block_size * N |
      -----------------------------------------------------
Value  |   0    | ... |      1     | ... |       N        |
      -----------------------------------------------------
Note   | Seq #0 |     |   Seq #1   |     |      Seq #N    |
```

To analyse the received file the following pseudocode will be use

```{python}
list_of_sequence = []

for sequence_number in file_received[::block_size]:
    list_of_sequence.append(sequence_number)

check_if_list_is_sequential(list_of_sequence)
```

### Throughput

In order to calculate the throughput the master starts a timer by using chrono.h library just before starting to send the file to the slaves. The master the stops the timer when it has finished sending to all slaves.

Afterward the then saves the two values into a test_scenario_#_times.json

The json is formatted as bellow.
```{json}
{
    "start_time": chrono_start_time,
    "end_time": chrono_end_time
}
```


## Test that will deliver the measurements

In order to make the test, follow the procedure below.

1. Run prepare_measurements.py on your pc. [Description of file](#prepare_measurementspy)
2. Build the project on all slaves.
3. Start run executable "test_qos_opt_slave" on all slaves
4. Configure the slave ip's in master_main.cpp.
5. Build the project on the master.
6. Start target "test_qos_opt_master" on the master
7. Run analyse_measurements.py on your pc. [Description of file](#analyse_measurementspy).

### prepare_measurements.py
This file will 
* Create the test file that are specific for each test.
* upload the project to the slaves and master. (Hopefully also make them build the project.)

### analyse_measurements.py
This retrieves all the logged files from the slaves and master, and then creates some diagrams and tables from the data.

## Description of test

As it is wanted to find the effect of the test parameters on the measurements a few 
different iteration of the main test will be done. In order to detect packet out of order a test
file will be generated specifically for the block_size.

Main test:
1. Select the specific file for this iteration of the test.
2. Master starts timer (To measure throughput)
3. Send the file to the slaves following the sequence shown in the diagram below.
4. Slave saves file one block at the time.
5. Master ends timer after the last block is sent, and saves the result.

<!--
```
@startuml main_test_sequence
server -> client_1: file[block_1]
hnote over server : send_delay
server -> client_2: file[block_1]
hnote over server : send_delay
...
server -> client_8: file[block_1]
hnote over server : send_delay
...
server -> client_1: file[block_end]
hnote over server : send_delay
server -> client_2: file[block_end]
hnote over server : send_delay
...
server -> client_8: file[block_end]
@enduml
```
-->

![](diagrams/main_test_sequence.svg)

## Test scenarios

* send_delay = [0, 20, 50, 70, 100, 130]
* block_size = [4096, 2048, 1024, 512, 256, 128, 64]

test_scenario(send_delay, block_size)

| Scenario # | send_delay [us] | block_size [Bytes] |
|---|---|---|
| 1 | 0 | 4096 |
| 2 | 20 | 4096 |
| 3 | 50 | 4096 |
| 4 | 70 | 4096 |
| 5 | 100 | 4096 |
| 6 | 130 | 4096 |
| 7 | 0 | 2048 |
| 8 | 20 | 2048 |
| 9 | 50 | 2048 |
| 10 | 70 | 2048 |
| 11 | 100 | 2048 |
| 12 | 130 | 2048 |
| 13 | 0 | 1024 |
| 14 | 10 | 1024 |
| 15 | 50 | 1024 |
| 16 | 70 | 1024 |
| 17 | 100 | 1024 |
| 18 | 130 | 1024 |
| 19 | 0 | 512 |
| 20 | 10 | 512 |
| 21 | 50 | 512 |
| 22 | 70 | 512 |
| 23 | 100 | 512 |
| 24 | 130 | 512 |
| 25 | 0 | 256 |
| 26 | 10 | 256 |
| 27 | 50 | 256 |
| 28 | 70 | 256 |
| 29 | 100 | 256 |
| 30 | 130 | 256 |
| 31 | 0 | 128 |
| 32 | 10 | 128 |
| 33 | 50 | 128 |
| 34 | 70 | 128 |
| 35 | 100 | 128 |
| 36 | 130 | 128 |
| 37 | 0 | 64 |
| 38 | 10 | 64 |
| 39 | 50 | 64 |
| 40 | 70 | 64 |
| 41 | 100 | 64 |
| 42 | 130 | 64 |

## Dependencies
| Dependency version | Version number |
|---|---|
|DataTransport|0.3.0|

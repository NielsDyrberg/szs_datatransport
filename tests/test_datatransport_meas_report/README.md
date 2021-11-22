<!-- 
To compile puml use: (Assuming plantuml you are in the directory)
plantuml.jar -tsvg README.md -o diagrams
-->


# Test on the DataTransport package

## Todo

## What measurements are interesting

| Measurement | Description |
|---|---|
| Delay | Test the round-trip/2, in order to know it |
| Jitter | When testing round-trip time log the jitter |

## Test that will deliver the measurements

One test should be able to deliver both measurements.

1. Start target "test_datatransport_slave" on all slaves
2. Configure the slave ip's in master_main.cpp.
3. Start target "test_datatransport_master" on the master
4. Run analyse_measurements.py

## Description of test
Main test (Repeat 100 times):
1. Start timer
2. Send a ping signal to all slaves (one slave at the time).
3. Log the time for ping ack.
4. Repeat 100 times

<!--
```
@startuml main_test_sequence
server -> client_1: ping
server <- client_1: ping_ack
server -> client_2: ping
server <- client_2: ping_ack
server -> client_N: ping
server <- client_N: ping_ack
@enduml
```
-->

![](diagrams/main_test_sequence.svg)


## Dependencies
| Dependency version | Version number |
|---|---|
|DataTransport|0.3.0|

## Test files

Test files have been found at:
[thinkbroadband.com](https://www.thinkbroadband.com/download)

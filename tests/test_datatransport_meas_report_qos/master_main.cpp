//
// Created by ncpd on 19-11-2021.
//

#include <iostream>
#include <unistd.h>
#include <chrono>
#include <fstream>
#include <unistd.h>
#include "udp_client.h"
#include "shared_types.h"

#define TEST_CIRCLES 200
#define NUMBER_OF_SLAVES 1
#define PORT 1695

static char* slave_ips[] = {"192.168.1.47", nullptr, nullptr, nullptr,
                            nullptr, nullptr, nullptr, nullptr};
const uint16_t buffer_size = 4096;
uint8_t comm_buffer[buffer_size];
UDP_client* slaves;

long long int measured_data[NUMBER_OF_SLAVES][TEST_CIRCLES] = {0};

int add_slave(int slave_number) {
    int err;

    if (slave_number >= NUMBER_OF_SLAVES){
        std::cout << "There can only be 8 slaves, [SZS_server, add_slave(int slave_number, char* host, bool is_ip)]" << std::endl;
        return -1;
    }
    slaves[slave_number] = UDP_client(SZP, slave_ips[slave_number], PORT, true, comm_buffer, buffer_size);

    return 0;
}

void ping_all_slaves(uint16_t iteration){
    std::chrono::steady_clock::time_point t1, t2;
    int16_t rsv_msg;

    for (int i = 0; i < NUMBER_OF_SLAVES; i++) {
        usleep(50);

        t1 = std::chrono::steady_clock::now();
        rsv_msg = slaves[i].send_and_receive(PING_REQUEST);
        if(rsv_msg == 1){
            // todo save return time
            t2 = std::chrono::steady_clock::now();
            auto time_span = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1);
            measured_data[i][iteration] = time_span.count();

        } else if (rsv_msg == -2){
            // todo nothing received
        } else {
            // Send or something error happened
            i--;
        }
    }
}

void save_data(){
    std::fstream f;

    f.open("/tmp/measured_data.csv", std::ios::out);

    for (int z = 0; z < TEST_CIRCLES; z++) {
        for (int i = 0; i < NUMBER_OF_SLAVES - 1; i++) {
            f << measured_data[i][z] << ", ";
        }
        f << measured_data[NUMBER_OF_SLAVES-1][z] << "\n";
    }

    f.close();
}

void setup() {
    int err;

    // Setup slaves
    for (int i = 0; i < NUMBER_OF_SLAVES; i++) {
        err = add_slave(i);
        if(err < 0){
            std::cout << "Slave " << i << " did not setup, [SZS_server, setup()]" << std::endl;
        }
    }
}

void test_qos_dt(){
    slaves = new UDP_client[NUMBER_OF_SLAVES];

    std::cout << "Setting up slaves" << std::endl;

    setup();

    sleep(1);

    std::cout << "Starting test" << std::endl;

    for(uint16_t i = 0; i < TEST_CIRCLES; i++){
        ping_all_slaves(i);
    }

    std::cout << "Test ended. Saving data ..." << std::endl;

    save_data();

    std::cout << "Data saved; test ended!" << std::endl;

}

int main(){
    test_qos_dt();
    return 0;
}
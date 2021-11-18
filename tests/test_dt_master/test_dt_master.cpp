//
// Created by ncpd on 05-11-2021.
//

#include <iostream>
#include <unistd.h>
#include "udp_client.h"

char host[] = "192.168.1.46";

int port_1 = 1695;
const uint16_t buffer_size_1 = 100;
uint8_t comm_buffer_1[buffer_size_1];

int port_2 = 1696;
const uint16_t buffer_size_2 = 100;
uint8_t comm_buffer_2[buffer_size_2];

[[noreturn]] void test_send_continuous(){
    uint16_t size;
    uint8_t msg[] = {0x45, 0x69, 0x6e, 0x67};

    UDP_client dt_1(SZP, host, port_1, true, comm_buffer_1, buffer_size_1);
    UDP_client dt_2(SYNC, host, port_2, true, comm_buffer_2, buffer_size_2);

    while (true){
        if(dt_1.send_and_receive(msg, sizeof(msg)) > 0) {
            size = dt_1.get_buffer();
            for (int i = 0; i < size; i++) {
                std::cout << unsigned(comm_buffer_1[i]) << std::endl;
            }
        }

        if(dt_2.send_and_receive(msg, sizeof(msg)) > 0) {
            size = dt_2.get_buffer();
            for (int i = 0; i < size; i++) {
                std::cout << unsigned(comm_buffer_1[i]) << std::endl;
            }
        }

        std::cout << "Master rerun" << std::endl;
        //sleep(1);
    }
}

int main(){
    test_send_continuous();
    return 0;
}
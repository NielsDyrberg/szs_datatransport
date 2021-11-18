//
// Created by ncpd on 05-11-2021.
//
#include <iostream>
#include "udp_server.h"

int port_1 = 1695;
const uint16_t buffer_size_1 = 100;
uint8_t comm_buffer_1[buffer_size_1];

int port_2 = 1696;
const uint16_t buffer_size_2 = 100;
uint8_t comm_buffer_2[buffer_size_2];

[[noreturn]] void test_recv_random_data(){
    uint16_t size;
    uint8_t msg[] = {0x50, 0x6f, 0x6e, 0x67};

    UDP_server dt_1(SZP, port_1, comm_buffer_1, buffer_size_1);
    UDP_server dt_2(SYNC, port_2, comm_buffer_2, buffer_size_2);

    while (true) {
        if (dt_1.receive(false) > 0) {
            size = dt_1.get_buffer();
            if (comm_buffer_1[0] == 0x45) {
                dt_1.send(msg, sizeof(msg));
            }
            for (int i = 0; i < size; i++) {
                std::cout << unsigned(comm_buffer_1[i]) << std::endl;
            }
            std::cout << "data" << std::endl;
        } else{
            std::cout << "Client rerun" << std::endl;
        }

        if (dt_2.receive(false) > 0) {
            size = dt_2.get_buffer();
            if (comm_buffer_1[0] == 0x45) {
                dt_2.send(msg, sizeof(msg));
            }
            for (int i = 0; i < size; i++) {
                std::cout << unsigned(comm_buffer_1[i]) << std::endl;
            }
            std::cout << "data" << std::endl;
        } else{
            std::cout << "Client rerun" << std::endl;
        }
    }
}

int main(){
    test_recv_random_data();
    return 0;
}
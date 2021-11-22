//
// Created by ncpd on 19-11-2021.
//

#include <iostream>
#include "udp_server.h"
#include "shared_types.h"

int port = 1695;
const uint16_t buffer_size = 4096;
uint8_t comm_buffer[buffer_size];

void receive_5mb(){
    uint16_t size;

    UDP_server dt(SZP, port, comm_buffer, buffer_size);

    while () {
        if (dt.receive(false) > 0) {
            size = dt.get_buffer();
            if (comm_buffer[0] == 0x45) {
                dt.send(msg, sizeof(msg));
            }
            for (int i = 0; i < size; i++) {
                std::cout << unsigned(comm_buffer[i]) << std::endl;
            }
            std::cout << "data" << std::endl;
        } else{
            std::cout << "Client rerun" << std::endl;
        }
    }
}

int main(){


    return 0;
}
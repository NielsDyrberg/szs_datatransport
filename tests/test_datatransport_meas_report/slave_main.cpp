//
// Created by ncpd on 19-11-2021.
//

#include <iostream>
#include "udp_server.h"
#include "shared_types.h"

int port = 1695;
const uint16_t buffer_size = 4096;
uint8_t comm_buffer[buffer_size];

[[noreturn]] void receive_and_responde_ping(){
    uint16_t size;

    UDP_server dt(SZP, port, comm_buffer, buffer_size);

    while (true) {
        size = dt.receive();
        if(size == 1){
            if (comm_buffer[0] == PING_REQUEST) {
                dt.send(PING_RESPONSE);
            }
        }
        std::cout << "Client rerun" << std::endl;
    }
}

int main(){
    receive_and_responde_ping();
    return 0;
}
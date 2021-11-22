//
// Created by ncpd on 19-11-2021.
//

#include <iostream>
#include <unistd.h>
#include "udp_client.h"
#include "shared_types.h"

char host[] = "192.168.1.46";

int port = 1695;
const uint16_t buffer_size = 4096;
uint8_t comm_buffer[buffer_size];

void send_5mb(){
    uint16_t size;
    UDP_client dt(SZP, host, port, true, comm_buffer, buffer_size);

    while (){
        if(dt.send(msg, sizeof(msg)) > 0) {
        }
    }
}

int main(){
    return 0;
}
//
// Created by ncpd on 05-11-2021.
//

#include <iostream>
#include <unistd.h>
#include "udp_client.h"

char host[] = "192.168.1.36";
int port = 1695;


[[noreturn]] void test_send_continuous(){
    uint8_t * bufPTR = nullptr;
    uint16_t size = 0;
    UDP_client dt(host, port, true);
    uint8_t msg[] = {0x50, 0x69, 0x6e, 0x67};

    while (true){
        if(dt.send_and_receive(msg, sizeof(msg)) > 0) {
            bufPTR = dt.GetBuffer(bufPTR, &size);
            for (int i = 0; i < size; i++) {
                std::cout << unsigned(*bufPTR) << std::endl;
                bufPTR++;
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
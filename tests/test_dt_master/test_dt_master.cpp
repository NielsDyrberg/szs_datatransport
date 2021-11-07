//
// Created by ncpd on 05-11-2021.
//

#include <iostream>
#include <unistd.h>
#include "udp_client.h"

char host[] = "192.168.1.41";
int port = 1695;


[[noreturn]] void test_send_continuous(){
    uint8_t * bufPTR = nullptr;
    uint16_t size = 0;
    UDP_client dt(host, port, true);

    while (true){
        if(dt.send(0xF1) > 0) {
            /*bufPTR = dt.GetBuffer(bufPTR, &size);
            for (int i = 0; i < size; i++) {
                std::cout << unsigned(*bufPTR) << std::endl;
                bufPTR++;
            }*/
        }
        sleep(1);
    }
}

int main(){
    test_send_continuous();
    return 0;
}
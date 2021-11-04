//
// Created by ncpd on 31-10-2021.
//

#include <iostream>
#include <unistd.h>
#include "dataTransport.h"

char host[] = "192.168.0.105";
int port = 1695;


void test_send(){
    DataTransport dt(host, port, true);
    dt.open_connection();
    uint8_t msgToSend[] = {255, 3, 53, 35, 42};
    dt.send(msgToSend, sizeof(msgToSend));
}

void test_send_longlongint(){
    DataTransport dt(host, port, true);
    dt.open_connection();
    long long unsigned int msgToSend[] = {1229782938247303441, 2459565876494606882};
    dt.send(msgToSend, sizeof(msgToSend));
}

[[noreturn]] void test_send_continuous(){
    DataTransport dt(host, port, true);
    dt.open_connection();
    uint8_t msgToSend[] = {1, 2, 43, 2, 1, 2, 2, 3, 1, 12, 2, 31, 2, 3};
    while (true){
        dt.send(msgToSend, sizeof(msgToSend));
        std::cout << "yes" << std::endl;
        sleep(1);
    }
}

int main(){
    test_send_continuous();
    return 0;
}
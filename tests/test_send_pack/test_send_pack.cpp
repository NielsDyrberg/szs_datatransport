//
// Created by ncpd on 31-10-2021.
//

#include <iostream>
#include "dataTransport.h"

void test_send(){
    char hostname[] = "10.103.8.129";
    DataTransport dt(hostname, 1695, true);
    // char* tmp_hostname = dt.GetHostname();
    dt.open_connection();
    uint8_t msgToSend[] = {255, 3, 53, 35, 42};
    dt.send(msgToSend, sizeof(msgToSend));
}

void test_send_longlongint(){
    char hostname[] = "10.103.8.129";
    DataTransport dt(hostname, 1695, true);
    // char* tmp_hostname = dt.GetHostname();
    dt.open_connection();
    long long unsigned int msgToSend[] = {1229782938247303441, 2459565876494606882};
    dt.send(msgToSend, sizeof(msgToSend));
}

void test_recv_random_data(){
    uint8_t * bufPTR = nullptr;
    uint16_t size = 0;
    DataTransport dt;
    if(dt.receive() > 0) {
        bufPTR = dt.GetBuffer(bufPTR, &size);
        for (int i = 0; i < size; i++) {
            std::cout << unsigned(*bufPTR) << std::endl;
            bufPTR++;
        }
    }
    std::cout << "Hello, World!" << std::endl;
}

void test_recv_two_longlong_int(){
    long long unsigned int* bufPTR = nullptr;
    uint8_t size = 0;
    DataTransport dt;
    if(dt.receive() > 0) {
        bufPTR = dt.GetBuffer(bufPTR, &size);
        for (int i = 0; i < size; i++) {
            std::cout << *bufPTR << std::endl;
            bufPTR++;
        }
    }
    std::cout << "Hello, World!" << std::endl;
}

int main(){
    test_recv_two_longlong_int();
    return 0;
}
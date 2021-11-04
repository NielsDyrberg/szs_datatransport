//
// Created by ncpd on 03-11-2021.
//
#include <iostream>
#include "dataTransport.h"

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


[[noreturn]] void test_recv_random_data(){
    uint8_t * bufPTR = nullptr;
    uint16_t size = 0;
    DataTransport dt;
    while(1){
        if(dt.receive() > 0) {
            bufPTR = dt.GetBuffer(bufPTR, &size);
            for (int i = 0; i < size; i++) {
                std::cout << unsigned(*bufPTR) << std::endl;
                bufPTR++;
            }
        }
    }
    std::cout << "Hello, World!" << std::endl;
}

int main(){
    test_recv_random_data();
    return 0;
}

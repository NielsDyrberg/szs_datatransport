//
// Created by ncpd on 22-11-2021.
//

#include <iostream>
#include <fstream>
#include "udp_server.h"
#include "shared_types.h"

#define PORT 1695
const uint16_t buffer_size = 8192;
uint8_t comm_buffer[buffer_size];
uint16_t bytes_read;

std::ofstream fd;

void close_test_file(){
    std::cout << "Close file" << std::endl;
    fd.close();
}

void save_to_test_file(){
    fd.write((char* )&comm_buffer[1], bytes_read-1);
}

void open_new_test_file(uint8_t block_i, uint8_t delay_i){
    // create the file path
    std::string file_path = rcv_files_path+"rcv_file_"+block_size_char[block_i]+"_"+send_delay_char[delay_i]+".bin";
    const char * c_file_path = file_path.c_str();

    std::cout << "opens : " << file_path << std::endl;

    // Open file
    fd.open(c_file_path, std::ios::binary);
}

[[noreturn]] void receive(){
    UDP_server dt(SZP, PORT, comm_buffer, buffer_size);

    while (true) {
        bytes_read = dt.receive();
        if(bytes_read < 1){
            continue;
        }
        switch (comm_buffer[0]) {
            case NEW_FILE:
                if(bytes_read == 3) {
                    open_new_test_file(comm_buffer[1], comm_buffer[2]);
                }
                break;
            case PACKET:
                save_to_test_file();
                break;
            case FILE_END:
                close_test_file();
                break;

        }
    }
}

int main(){
    receive();
    return 0;
}
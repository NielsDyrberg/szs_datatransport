//
// Created by ncpd on 22-11-2021.
//

#include <iostream>
#include <cstring>
#include <string>
#include <fstream>
#include <unistd.h>
#include <chrono>
#include "udp_client.h"
#include "shared_types.h"

/***********************************************************************************************************************
 * Defines
 **********************************************************************************************************************/

#define NUMBER_OF_SLAVES 8
#define PORT 1695

/***********************************************************************************************************************
 * Static variables
 **********************************************************************************************************************/

const int buffer_size = 8192;
uint8_t buffer[buffer_size] = {0};

FILE* file_fd;

UDP_client* slaves;
static char* slave_ips[] = {"192.168.0.131", "192.168.0.132", "192.168.0.133", "192.168.0.100",
                            "192.168.0.103", "192.168.0.105", "192.168.0.104", "192.168.0.106"};
long long int play_time = 0;

const int iterations = n_block_size * n_send_delay;
long long int measured_data[iterations] = {0};
int index_iteration = 0;

/***********************************************************************************************************************
 * Test functions
 **********************************************************************************************************************/

void send_file(int block_i, int delay_i){
    std::chrono::steady_clock::time_point t1, t2;
    unsigned int bytes_read = 0;

    // Send new file request
    for (int i = 0; i < NUMBER_OF_SLAVES; i++) {
        uint8_t msg[] = {NEW_FILE, (uint8_t)block_i, (uint8_t)delay_i};
        slaves[i].send(msg, sizeof(msg));
    }
    usleep(1000);

    t1 = std::chrono::steady_clock::now();
    while (true){
        buffer[0] = PACKET;
        bytes_read = fread(&buffer[1], 1, block_size[block_i], file_fd);
        if (bytes_read > 0){
            for (int i = 0; i < NUMBER_OF_SLAVES; i++) {
                slaves[i].send(buffer, bytes_read+1);
                usleep(send_delay[delay_i]);
            }
        } else{
            t2 = std::chrono::steady_clock::now();
            auto time_span = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1);
            measured_data[index_iteration++] = time_span.count();
            break;
        }
    }

    usleep(1000);
    fclose(file_fd);
    std::cout << "Song end" << std::endl;
    // Send file end request
    for (int i = 0; i < NUMBER_OF_SLAVES; i++) {
        slaves[i].send(FILE_END);
    }

}

void open_file(int block_i){
    // create the file path
    std::string file_path = test_files_path+"send_file_"+block_size_char[block_i]+".bin";
    const char * c_file_path = file_path.c_str();

    file_fd = fopen(c_file_path, "rb");
    if(file_fd == nullptr){
        std::cout << "Did not load song, [SZS_server, load_song(char *song_path)]" << std::endl;
        exit(1);
    }
    std::cout << "file opened: " << c_file_path << std::endl;
}

int add_slave(int slave_number) {
    int err;

    if (slave_number >= NUMBER_OF_SLAVES){
        std::cout << "There can only be 8 slaves, [SZS_server, add_slave(int slave_number, char* host, bool is_ip)]" << std::endl;
        return -1;
    }
    slaves[slave_number] = UDP_client(SZP, slave_ips[slave_number], PORT, true, buffer, buffer_size);

    return 0;
}

void save_data(){
    std::fstream f;

    f.open("/tmp/measured_qos_opt_data.csv", std::ios::out);

    for (int i = 0; i < iterations - 1; i++) {
        f << measured_data[i] << ", ";
    }

    f.close();
}

void run_all_test_iterations(){
    for (int j = 0; j < n_send_delay; j++) {
        for (int i = 0; i < n_block_size; i++) {
            open_file(i);
            send_file(i, j);

            usleep(1000);
        }
    }

}

void setup(){
    int err;
    slaves = new UDP_client[NUMBER_OF_SLAVES];

    // Setup slaves
    for (int i = 0; i < NUMBER_OF_SLAVES; i++) {
        err = add_slave(i);
        if(err < 0){
            std::cout << "Slave " << i << " did not setup, [SZS_server, setup()]" << std::endl;
        }
    }
}

/***********************************************************************************************************************
 * Main function
 **********************************************************************************************************************/

int main(){
    setup();
    run_all_test_iterations();
    save_data();
    return 0;
}

//
// Created by ncpd on 21-10-2021.
//

#include "dataTransport.h"
#include <iostream>

#define BUFFER_LEN 1028  // Length of rx buffer

int DataTransport::listen_s = 0;
bool DataTransport::listen_socket_set = false;

/**********************************************************************************************************************
 * Public methods
 **********************************************************************************************************************/

DataTransport::DataTransport() {
    this->timeout_sec = 0;
    this->timeout_usec = 0;
    this->p_buffer = nullptr;
    this->buffer_len = 0;
    this->port = 0;
    this->s = 0;
}

DataTransport::DataTransport(unsigned int port) {
    this->timeout_sec = 5;
    this->timeout_usec = 0;
    this->p_buffer = new uint8_t[BUFFER_LEN];
    this->buffer_len = BUFFER_LEN;
    this->port = port;

    if(( this->s = socket(ADDRESS_FAMILY, SOCK_TYPE, IPPROTO_UDP) )< 0)
    {
        printf("Cannot create socket, [DataTransport.cpp, DataTransport()]\n");
    }

    if(!listen_socket_set) {
        if ((listen_s = socket(ADDRESS_FAMILY, SOCK_TYPE, IPPROTO_UDP)) < 0) {
            printf("Cannot create socket, [DataTransport.cpp, DataTransport()]\n");
        }
        set_listen_addr();
        listen_socket_set = true;
    }
}

/**********************************************************************************************************************/

DataTransport::DataTransport(unsigned int port, uint8_t* buffer, uint16_t buffer_size){
    this->timeout_sec = 5;
    this->timeout_usec = 0;
    this->p_buffer = buffer;
    this->buffer_len = buffer_size;
    this->port = port;

    if(( this->s = socket(ADDRESS_FAMILY, SOCK_TYPE, IPPROTO_UDP) )< 0)
    {
        printf("Cannot create socket, [DataTransport.cpp, DataTransport()]\n");
    }

    if(!listen_socket_set) {
        if ((listen_s = socket(ADDRESS_FAMILY, SOCK_TYPE, IPPROTO_UDP)) < 0) {
            printf("Cannot create socket, [DataTransport.cpp, DataTransport()]\n");
        }
        set_listen_addr();
        listen_socket_set = true;
    }
}

/**********************************************************************************************************************/

int16_t DataTransport::receive(bool timeout, struct sockaddr_in* addr, socklen_t* addr_size){
    int err = 0;
    for(;;){
        if(timeout){
            err = timeout_handler();
            if (err < 0){
                this->bytes_recv = -1;
                break;
            }
        }

        this->bytes_recv = recvfrom(listen_s, p_buffer, this->buffer_len, 0, (struct sockaddr *)addr, addr_size);
        if(this->bytes_recv < 0){
            // ignore error
            continue;
        }

        break;
    }

    if (this->bytes_recv < 0){
        std::cout <<
        "Error happened, [datatransport.cpp, receive(struct sockaddr_in* addr, socklen_t* addr_size, bool timeout)]"
        << std::endl;
        for (int i = 0; i < this->bytes_recv; i++) {
            std::cout << unsigned(*p_buffer) << std::endl;
            p_buffer[i];
        }
    }
    if(addr != nullptr){
        addr->sin_port = htons( port );
    }
    return this->bytes_recv;
}

/**********************************************************************************************************************/

int DataTransport::send(uint8_t msg, struct sockaddr_in* addr, const socklen_t* addr_size) const{
    return sendto(s , &msg , sizeof(uint8_t), 0, (struct sockaddr*)addr, *addr_size);
}


int DataTransport::send(const uint8_t* msg, uint16_t msg_size, struct sockaddr_in* addr, const socklen_t* addr_size) const{
    return sendto(s , msg , msg_size, 0, (struct sockaddr*)addr, *addr_size);
}

/**********************************************************************************************************************/

int DataTransport::send(const long long unsigned int *msg, uint8_t msg_size, struct sockaddr_in* addr, const socklen_t* addr_size) const{
    return sendto(s , msg , msg_size, 0, (struct sockaddr*)addr, *addr_size);
}

/**********************************************************************************************************************/

uint8_t* DataTransport::GetBuffer(uint8_t* buff, uint16_t* size){
    buff = &p_buffer[0];
    *size = this->bytes_recv;
    return buff;
}

/**********************************************************************************************************************/

long long unsigned int* DataTransport::GetBuffer(long long unsigned int* buff, uint8_t* size){
    buff = (long long unsigned int*)&p_buffer[0];
    *size = this->bytes_recv/ sizeof(long long unsigned int);
    return buff;
}

/**********************************************************************************************************************/

uint16_t DataTransport::get_buffer() {
    return bytes_recv;
}

/**********************************************************************************************************************/

int DataTransport::set_timeout_len(unsigned int sec, unsigned int usec){
    if(sec > 60 or usec > 1000){
        std::cout <<
        "sec or usec out of range, [Datatransport.cpp, set_timeout_len(unsigned int sec, unsigned int usec)]"
        << std::endl;
        return -1;
    }

    timeout_sec = (__time_t) sec;
    timeout_usec = (__suseconds_t) usec;
    return 0;
}

/**********************************************************************************************************************
 * Private methods
 **********************************************************************************************************************/

int DataTransport::timeout_handler() {
    struct timeval tv{};
    fd_set set;
    int retval;

    /* Watch stdin (fd 0) to see when it has input. */
    FD_ZERO(&set);
    FD_SET(listen_s, &set);

    /* Wait up to five seconds. */
    tv.tv_sec = timeout_sec;
    tv.tv_usec = timeout_usec;

    retval = select(listen_s+1, &set, nullptr, nullptr, &tv);
    /* Don't rely on the value of tv now! */

    if (retval == -1){
        perror("select()");
        return -1;
    }
    else if (retval){
        // printf("Data is available now.\n");
        /* FD_ISSET(0, &rfds) will be true. */
        return 0;
    }
    else {
        printf("No data within five seconds, [dataTransport.cpp, timeout_handler()].\n");
        return -2;
    }
}

int DataTransport::set_listen_addr() const {
    struct sockaddr_in tmp_listen_addr{};

    bzero(&tmp_listen_addr, sizeof(tmp_listen_addr));

    // Filling server information
    tmp_listen_addr.sin_family = AF_INET;
    tmp_listen_addr.sin_port = htons(port);
    tmp_listen_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    // Bind the socket with the server address
    if ( bind(listen_s, (const struct sockaddr *)&tmp_listen_addr, sizeof(tmp_listen_addr)) < 0 )
    {
        perror("bind failed, [dataTransport.cpp, set_listen_addr()]");
        exit(EXIT_FAILURE);
    }

    return 0;
}

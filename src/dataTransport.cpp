//
// Created by ncpd on 21-10-2021.
//

#include "dataTransport.h"

#include <iostream>
#include <cstddef>
#include <cstdint>
#include <algorithm>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "netinet/in.h"
#include <netdb.h>

/**********************************************************************************************************************
 * Public methods
 **********************************************************************************************************************/

DataTransport::DataTransport() {
    this->buffer_len = BUFFER_LEN;
    this->dst_hostname = nullptr;
    this->port = PORT;

    this->src_addr = {};
    this->src_storage = {};
    this->src_addr_size = {};
    this->src_s = socket(AF_INET, SOCK_TYPE, 0);

    this->dst_addr = {};
    this->dst_storage = {};
    this->dst_addr_size = {};
    this->dst_s = socket(AF_INET, SOCK_TYPE, 0);

    this->portOpen = false;
    this->connected = false;
}

DataTransport::DataTransport(char* hostname) {
    this->buffer_len = BUFFER_LEN;
    this->dst_hostname = hostname;
    this->port = PORT;

    this->src_addr = {};
    this->src_storage = {};
    this->src_addr_size = {};
    this->src_s = socket(AF_INET, SOCK_TYPE, 0);

    this->dst_addr = {};
    this->dst_storage = {};
    this->dst_addr_size = {};
    this->dst_s = socket(AF_INET, SOCK_TYPE, 0);

    this->portOpen = false;
    this->connected = false;
}

DataTransport::DataTransport(char* hostname, unsigned int port){
    this->buffer_len = BUFFER_LEN;
    this->dst_hostname = nullptr;
    this->port = PORT;

    this->src_addr = {};
    this->src_storage = {};
    this->src_addr_size = {};
    this->src_s = socket(AF_INET, SOCK_TYPE, 0);

    this->dst_addr = {};
    this->dst_storage = {};
    this->dst_addr_size = {};
    this->dst_s = socket(AF_INET, SOCK_TYPE, 0);

    this->portOpen = false;
    this->connected = false;
}

DataTransport::DataTransport(unsigned int port, unsigned int bufferLen, unsigned int headerSize,
                             unsigned char addrFamily, unsigned  char socketType){
    this->buffer_len = bufferLen;
    this->dst_hostname = nullptr;
    this->port = port;

    this->src_addr = {};
    this->src_storage = {};
    this->src_addr_size = {};
    this->src_s = socket(AF_INET, SOCK_TYPE, 0);

    this->dst_addr = {};
    this->dst_storage = {};
    this->dst_addr_size = {};
    this->dst_s = socket(AF_INET, SOCK_TYPE, 0);

    this->portOpen = false;
    this->connected = false;
}

void DataTransport::close(){}

int DataTransport::open_connection(){
    int err;

    err = getHostByName();
    if(err < 0){
        // Error happened
        return -1;
    }

    dst_s = socket(AF_INET, SOCK_TYPE, 0);

    memset(&dst_addr, 0, sizeof(dst_addr));

    dst_addr.sin_family = AF_INET;
    dst_addr.sin_port = htons(port);

    // Convert IPv4 and IPv6 addresses from text to binary form
    //if(inet_pton(AF_INET, dst_addr, &serv_addr.sin_addr)<=0)
    if(inet_pton(AF_INET, 0, &dst_addr.sin_addr)<=0)
    {
        std::cout << "Invalid address/ Address not supported" << std::endl;
        exit(EXIT_FAILURE);
    }

    if (::connect(dst_s, (struct sockaddr *)&dst_addr, sizeof(dst_addr)) < 0)
    {
        printf("Connection Failed");
        exit(EXIT_FAILURE);
    }
    this->connected=true;

    return 0;
}

int16_t DataTransport::receive(){
    char host[NI_MAXHOST], service[NI_MAXSERV];

    if (!this->portOpen){
        this->open_port();
    }

    for(;;){
        this->bytes_recv = recvfrom(src_s, this->buffer, this->buffer_len, 0, (struct sockaddr *)&src_addr,
                                    &src_addr_size);
        if(this->bytes_recv < 0){
            // ignore error
            continue;
        }

        dst_s = getnameinfo((struct sockaddr *) &src_addr,
                            src_addr_size, host, NI_MAXHOST,
                            service, NI_MAXSERV, NI_NUMERICSERV);
        if (dst_s == 0)
            std::cout << "Received: " << (long) bytes_recv << " bytes from: " << host << " : " << service << std::endl;
        else
            fprintf(stderr, "getnameinfo: %s\n", gai_strerror(dst_s));

        break;
    }

    if (this->bytes_recv < 0){
        std::cout << "Error happend" << std::endl;
        for (int i = 0; i < this->bytes_recv; i++) {
            std::cout << unsigned(*this->buffer) << std::endl;
            this->buffer[i];
        }
    }

    return this->bytes_recv;
}

int DataTransport::send(uint8_t* msg, uint8_t size){
    if(!this->connected){
        return -1;
    }

    ::send(this->dst_s , msg , size , 0 );
    return 0;
}

char* DataTransport::GetHostname(){
    return this->dst_hostname;
}

uint8_t* DataTransport::GetBuffer(uint8_t* buff, uint16_t* size){
    buff = &this->buffer[0];
    *size = this->bytes_recv;
    return buff;
}

/**********************************************************************************************************************
 * Private methods
 **********************************************************************************************************************/

/**
 * Gets host (ip) from hostname
 * @return int
 * @retval -1, if unsuccessful
 * @retval 0, if successful
 */
int DataTransport::getHostByName() {
    if (dst_hostname == nullptr){return -1;}  // Return error if hostname is not set

    struct addrinfo hints = {}, *addrs;
    char port_str[16] = {};

    hints.ai_family = ADDRESS_FAMILY;
    hints.ai_socktype = SOCK_TYPE;
    hints.ai_protocol = IPPROTO_UDP;

    sprintf(port_str, "%d", port);

    dst_s = getaddrinfo(dst_hostname, port_str, &hints, &addrs);
    if (dst_s != 0)
    {
        fprintf(stderr, "%s: %s\n", dst_hostname, gai_strerror(dst_s));
        return -1;
        // abort();
    }
    // this->dst_addr = (char*)&addrs;
    return 0;
}

/**
 * Opens port to enable receiving.
 */
void DataTransport::open_port() {
    src_addr.sin_family = AF_INET;
    src_addr.sin_addr.s_addr = INADDR_ANY;
    src_addr.sin_port = htons( port );

    // Forcefully attaching socket to the port
    if (bind(src_s, (struct sockaddr *)&src_addr, sizeof(src_addr))<0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    portOpen = true;
}



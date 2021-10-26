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

DataTransport::DataTransport() {
    this->bufferLen = BUFFER_LEN;
    this->address = nullptr;
    this->hostname = nullptr;
    this->port = PORT;
    this->headerSize = HEADER_SIZE;
    this->addressFamily = ADDRESS_FAMILY;
    this->sockType = SOCK_TYPE;

    this->s = NULL;
    this->portOpen = false;
    this->connected = false;
}

DataTransport::DataTransport(unsigned int port) {
    this->bufferLen = BUFFER_LEN;
    this->address = nullptr;
    this->hostname = nullptr;
    this->port = port;
    this->headerSize = HEADER_SIZE;
    this->addressFamily = ADDRESS_FAMILY;
    this->sockType = SOCK_TYPE;

    this->s = NULL;
    this->portOpen = false;
    this->connected = false;
}

DataTransport::DataTransport(unsigned int port, unsigned int bufferLen, unsigned int headerSize,
                             unsigned char addrFamily, unsigned  char socketType){
    this->bufferLen = bufferLen;
    this->address = nullptr;
    this->hostname = nullptr;
    this->port = port;
    this->headerSize = headerSize;
    this->addressFamily = addrFamily;
    this->sockType = socketType;

    this->s = NULL;
    this->portOpen = false;
    this->connected = false;
}

int DataTransport::getHostByName() {
    if (hostname == nullptr){return -1;}  // Return error if hostname is not set

    int err;
    struct addrinfo hints = {}, *addrs;
    char port_str[16] = {};

    hints.ai_family = addressFamily;
    hints.ai_socktype = sockType;
    hints.ai_protocol = IPPROTO_UDP;

    sprintf(port_str, "%d", port);

    err = getaddrinfo(hostname, port_str, &hints, &addrs);
    if (err != 0)
    {
        fprintf(stderr, "%s: %s\n", hostname, gai_strerror(err));
        return -1;
        // abort();
    }

    return 0;
}

void DataTransport::open_port() {
    struct sockaddr_in addr{};

    s = socket(AF_INET, sockType, 0);

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons( PORT );

    // Forcefully attaching socket to the port 8080
    if (bind(s, (struct sockaddr *)&addr,
             sizeof(addr))<0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    portOpen = true;
}

void DataTransport::close(){}

int DataTransport::connect(){
    /*
     * self._s = socket.socket(self._ADDRESS_FAMILY, self._SOCK_TYPE)
     * self._s.connect((self._ADDR, self._PORT))
     * self._connected = True
     */
    struct sockaddr_in serv_addr{};
    int err;

    err = getHostByName();
    if(err < 0){
        // Error happened
        return -1;
    }

    s = socket(AF_INET, sockType, 0);

    memset(&serv_addr, 0, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, address, &serv_addr.sin_addr)<=0)
    {
        std::cout << "Invalid address/ Address not supported" << std::endl;
        exit(EXIT_FAILURE);
    }

    if (::connect(s, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("Connection Failed");
        exit(EXIT_FAILURE);
    }
    this->connected=true;

    return 0;
}

unsigned char DataTransport::receive(){

    if (!this->portOpen){
        this->open_port();
    }

    this->bytes_recv = recv(this->s, this->buffer, this->bufferLen, 0);

    return this->bytes_recv;
}

void DataTransport::send(uint8_t* msg, uint8_t size){
    if(!this->connected){
        this->connect();
    }

    ::send(this->s , msg , size , 0 );
}

char* DataTransport::GetHostname(){
    return this->hostname;
}

uint8_t* DataTransport::GetBuffer(uint8_t* buff, uint16_t* size){
    buff = &this->buffer[0];
    *size = this->bytes_recv;
    return buff;
}

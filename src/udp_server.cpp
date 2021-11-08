//
// Created by ncpd on 07-11-2021.
//

#include "udp_server.h"
#include <iostream>

UDP_server::UDP_server(unsigned int port): DataTransport(port) {
    this->port_open = false;
    this->known_client = false;
    //ser_addr_size = 0;
    cli_addr_size = 0;
}

/**********************************************************************************************************************/

int16_t UDP_server::receive(bool timeout) {
    if (!this->port_open){
        this->open_port();
    }

    known_client = true;
    return DataTransport::receive(timeout, &cli_addr, &cli_addr_size);
}

/**********************************************************************************************************************/

int UDP_server::send(uint8_t msg) {
    if(!this->known_client){
        return -1;
    }
    DataTransport::send(msg, &cli_addr, &cli_addr_size);
    return 0;
}

/**********************************************************************************************************************/

int UDP_server::send(const uint8_t* msg, uint16_t msg_size){
    if(!this->known_client){
        return -1;
    }
    DataTransport::send(msg, msg_size, &cli_addr, &cli_addr_size);
    return 0;
}

/**********************************************************************************************************************/

int UDP_server::send(const long long unsigned int *msg, uint8_t msg_size){
    if(!this->known_client){
        return -1;
    }
    DataTransport::send(msg, msg_size, &cli_addr, &cli_addr_size);
    return 0;
}

/**********************************************************************************************************************
 * Private methods
 **********************************************************************************************************************/

int UDP_server::open_port() {

    //bzero(&ser_addr, sizeof(ser_addr));
    bzero(&cli_addr, sizeof(cli_addr));

//    ser_addr.sin_family = ADDRESS_FAMILY;
//    ser_addr.sin_addr.s_addr = htonl(INADDR_ANY);
//    ser_addr.sin_port = htons( port );
//
//    // Bind the socket with the server address
//    if ( bind(s, (const struct sockaddr *)&ser_addr,
//              sizeof(ser_addr)) < 0 )
//    {
//        perror("bind failed, [udp_server.h, open_port()]");
//        exit(EXIT_FAILURE);
//    }
    cli_addr_size = sizeof(cli_addr);
    //ser_addr_size = sizeof(ser_addr);

    port_open = true;

    return 0;

}


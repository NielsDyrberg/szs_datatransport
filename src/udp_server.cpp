/***********************************************************************************************************************
 * @name UDP_server (Implementation)
 * @file udp_server.cpp
 * @author Niels Dyrberg
 * @date 07-11-2021
 *
 * Purpose:
 *      Implement UDP_server class
 **********************************************************************************************************************/

/**********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/

#include "udp_server.h"
#include <iostream>

/**********************************************************************************************************************
 * Public methods
 **********************************************************************************************************************/

UDP_server::UDP_server(dt_type_t type, unsigned int port) : DataTransport(type, port) {
    this->port_open = false;
    this->known_client = false;
    cli_addr_size = 0;
}

/**********************************************************************************************************************/

UDP_server::UDP_server(dt_type_t type, unsigned int port, uint8_t *buffer, uint16_t buffer_size) :
        DataTransport(type, port, buffer, buffer_size) {
    this->port_open = false;
    this->known_client = false;
    cli_addr_size = 0;
}

/**********************************************************************************************************************/

int16_t UDP_server::receive(bool timeout) {
    if (!this->port_open) {
        this->open_port();
    }

    known_client = true;
    return DataTransport::receive(timeout, &cli_addr, &cli_addr_size);
}

/**********************************************************************************************************************/

int UDP_server::send(uint8_t msg) {
    if (!this->known_client) {
        return -1;
    }
    return DataTransport::send(msg, &cli_addr, &cli_addr_size);
}

/**********************************************************************************************************************/

int UDP_server::send(const uint8_t *msg, uint16_t msg_size) {
    if (!this->known_client) {
        return -1;
    }
    return DataTransport::send(msg, msg_size, &cli_addr, &cli_addr_size);
}

/**********************************************************************************************************************/

int UDP_server::send(const long long unsigned int *msg, uint8_t msg_size) {
    if (!this->known_client) {
        return -1;
    }
    return DataTransport::send(msg, msg_size, &cli_addr, &cli_addr_size);
}

/**********************************************************************************************************************/

int UDP_server::send(uint16_t msg_size) {
    if (!this->known_client) {
        return -1;
    }
    return DataTransport::send(p_buffer, msg_size, &cli_addr, &cli_addr_size);
}

/**********************************************************************************************************************
 * Private methods
 **********************************************************************************************************************/

/**
 * @details Opens the port for listening.
 * @note !!Disclaimer!! This does not open the port anymore, but now just prepares #cli_addr for writting. But it is
 * still here to tie everything else together.
 * @return 0.
 */
int UDP_server::open_port() {
    bzero(&cli_addr, sizeof(cli_addr));
    cli_addr_size = sizeof(cli_addr);
    port_open = true;
    return 0;
}


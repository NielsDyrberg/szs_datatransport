//
// Created by ncpd on 07-11-2021.
//

#include "udp_server.h"
#include <iostream>

UDP_server::UDP_server(unsigned int port) {
    this->buffer_len = BUFFER_LEN;
    this->port = port;
    this->port_open = false;
    this->known_client = false;

    this->s = socket(ADDRESS_FAMILY, SOCK_TYPE, 0);

}

/**********************************************************************************************************************/

int16_t UDP_server::receive(bool timeout) {
    int err;

    if (!this->port_open){
        this->open_port();
    }

    for(;;){
        if(timeout){
            err = timeout_handler();
            if (err < 0){
                this->bytes_recv = -1;
                break;
            }
        }

        this->bytes_recv = recvfrom(s, this->buffer, this->buffer_len, 0, (struct sockaddr *)&cli_addr,
                                    &cli_addr_size);
        if(this->bytes_recv < 0){
            // ignore error
            continue;
        }

        break;
    }

    if (this->bytes_recv < 0){
        std::cout << "Error happened, datatransport.cpp" << std::endl;
        for (int i = 0; i < this->bytes_recv; i++) {
            std::cout << unsigned(*this->buffer) << std::endl;
            this->buffer[i];
        }
    }

    known_client = true;
    return this->bytes_recv;

}

/**********************************************************************************************************************/

int UDP_server::send(uint8_t msg) {
    if(!this->known_client){
        return -1;
    }

    sendto(this->s , &msg , sizeof(uint8_t), 0, (const struct sockaddr *) &cli_addr, cli_addr_size);

    return 0;
}

/**********************************************************************************************************************
 * Private methods
 **********************************************************************************************************************/

int UDP_server::timeout_handler() {
    struct timeval tv{};
    fd_set set;
    int retval;

    /* Watch stdin (fd 0) to see when it has input. */
    FD_ZERO(&set);
    FD_SET(s, &set);

    /* Wait up to five seconds. */
    tv.tv_sec = 5;
    tv.tv_usec = 0;

    retval = select(s+1, &set, nullptr, nullptr, &tv);
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
        printf("No data within five seconds.\n");
        return -2;
    }
}

int UDP_server::open_port() {

    bzero(&ser_addr, sizeof(ser_addr));
    bzero(&cli_addr, sizeof(cli_addr));

    ser_addr.sin_family = ADDRESS_FAMILY;
    ser_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    ser_addr.sin_port = htons( port );

    // Bind the socket with the server address
    if ( bind(s, (const struct sockaddr *)&ser_addr,
              sizeof(ser_addr)) < 0 )
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    cli_addr_size = sizeof(cli_addr);
    ser_addr_size = sizeof(ser_addr);

    port_open = true;

    return 0;

}


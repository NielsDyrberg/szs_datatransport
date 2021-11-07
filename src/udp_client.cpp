//
// Created by ncpd on 07-11-2021.
//

#include "udp_client.h"
#include <iostream>

UDP_client::UDP_client(char* host, unsigned int port, bool is_ip) {
    this->buffer_len = BUFFER_LEN;
    this->port = port;
    this->connected = false;

    this->ser_hostname = nullptr;
    this->ser_ip = nullptr;
    this->is_ip = is_ip;

    setHost(host);

    this->s = socket(ADDRESS_FAMILY, SOCK_TYPE, 0);
}

/**********************************************************************************************************************/

int16_t UDP_client::receive(bool timeout) {
    if (!this->connected){
        this->open_connection();
    }

    for(;;){
        if(timeout){
            timeout_handler();
        }

        this->bytes_recv = recvfrom(s, this->buffer, this->buffer_len, 0, (struct sockaddr *)&ser_addr,
                                    &ser_addr_size);
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

    return this->bytes_recv;

}

/**********************************************************************************************************************/

int UDP_client::send(uint8_t msg) {
    if (!this->connected){
        this->open_connection();
    }

    sendto(this->s , &msg , sizeof(uint8_t), 0, (const struct sockaddr *) &ser_addr, ser_addr_size);

    return 0;
}

/**********************************************************************************************************************
 * Private methods
 **********************************************************************************************************************/

/**
 * Sets the host variables, depending on if the host var is ip or hostname
 * @param host, Either ip or hostname of the host.
 * @return None
 */
void UDP_client::setHost(char *host){
    // Set either host ip or name
    if (is_ip){
        this->ser_ip = host;
        this->ser_hostname = nullptr;
    } else{
        this->ser_hostname = host;
        this->ser_ip = nullptr;
    }
}


int UDP_client::open_connection() {
    int err;

    // Get addr info
    if (is_ip) {
        err = get_host_by_ip();
    } else{
        //err = getHostByName();
        err = -1;
    }
    if(err < 0){
        // Error happened
        return -1;
    }

    // Bind the socket with the server address
//    if ( bind(s, (const struct sockaddr *)&ser_addr,
//              sizeof(ser_addr)) < 0 )
//    {
//        perror("bind failed");
//        exit(EXIT_FAILURE);
//    }

    if(connect(s, (struct sockaddr *)&ser_addr, sizeof(ser_addr)) < 0)
    {
        printf("\n Error : Connect Failed \n");
        exit(0);
    }

    ser_addr_size = sizeof(ser_addr);
    connected = true;
    return 0;

}

int UDP_client::timeout_handler() {
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

int UDP_client::get_host_by_ip(){
    if (ser_ip == nullptr){return -1;}  // Return error if ip is not set

    bzero(&ser_addr, sizeof(ser_addr));

    ser_addr.sin_family = ADDRESS_FAMILY;
    ser_addr.sin_port = htons( port );

    // Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(ADDRESS_FAMILY, ser_ip, &ser_addr.sin_addr)<=0)
    {
        printf("Invalid address/ Address not supported");
        return -1;
    }

    return 0;
}
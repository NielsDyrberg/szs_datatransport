//
// Created by ncpd on 07-11-2021.
//

#include "udp_client.h"
#include <iostream>

UDP_client::UDP_client(char* host, unsigned int port, bool is_ip): DataTransport(port) {
    this->known_host = false;

    this->ser_hostname = nullptr;
    this->ser_ip = nullptr;
    this->is_ip = is_ip;

    setHost(host);
}

/**********************************************************************************************************************/

UDP_client::UDP_client(char* host, unsigned int port, bool is_ip, uint8_t* buffer, uint16_t buffer_size):
DataTransport(port, buffer, buffer_size) {

}

/**********************************************************************************************************************/

int16_t UDP_client::receive(bool timeout) {
    if (!this->known_host){
        std::cout << "Host not known, [udp_client.cpp, receive(bool timeout)]" << std::endl;
        return -1;
    }

    return DataTransport::receive(timeout);
}

/**********************************************************************************************************************/

int UDP_client::send(uint8_t msg) {
    if (!this->known_host){
        std::cout << "Host not known, [udp_client.cpp, send(uint8_t msg)]" << std::endl;
        return -1;
    }
    return DataTransport::send(msg, &ser_addr, &ser_addr_size);
}

/**********************************************************************************************************************/

int UDP_client::send(const uint8_t* msg, uint16_t msg_size){
    if (!this->known_host){
        std::cout << "Host not known, [udp_client.cpp, send(const uint8_t* msg, uint16_t msg_size)]" << std::endl;
        return -1;
    }
    return DataTransport::send(msg, msg_size, &ser_addr, &ser_addr_size);
}

/**********************************************************************************************************************/

int UDP_client::send(const long long unsigned int *msg, uint8_t msg_size){
    if (!this->known_host){
        std::cout << "Host not known, [udp_client.cpp, send(msg, msg_size, &ser_addr, &ser_addr_size)]" << std::endl;
        return -1;
    }
    return DataTransport::send(msg, msg_size, &ser_addr, &ser_addr_size);
}

/**********************************************************************************************************************/

int16_t UDP_client::send_and_receive(uint8_t msg) {
    int16_t err;
    err = (int16_t)this->send(msg);
    if(err < 0){
        std::cout << "Send failed, [udp_client.cpp, send_and_receive(uint8_t msg)]" << std::endl;
        return -1;
    }

    err = this->receive(true);
    if(err < 0){
        std::cout << "Receive failed, [udp_client.cpp, send_and_receive(uint8_t msg)]" << std::endl;
        return -2;
    }

    return err;
}

/**********************************************************************************************************************/

int16_t UDP_client::send_and_receive(uint8_t *msg, uint16_t size) {
    int16_t err;
    err = (int16_t)this->send(msg, size);
    if(err < 0){
        std::cout << "Send failed" << std::endl;
        return -1;
    }

    err = this->receive(true);
    if(err < 0){
        std::cout << "Receive failed" << std::endl;
        return -2;
    }

    return err;
}

/**********************************************************************************************************************
 * Private methods
 **********************************************************************************************************************/

/**
 * Sets the host variables, depending on if the host var is ip or hostname
 * @param host, Either ip or hostname of the host.
 * @return None
 */
int UDP_client::setHost(char *host){
    int err;

    // Set either host ip or name
    if (is_ip){
        this->ser_ip = host;
        this->ser_hostname = nullptr;
        err = get_host_by_ip();
    } else{
        this->ser_hostname = host;
        this->ser_ip = nullptr;
        err = -1;
    }
    if(err < 0){
        // Error happened
        return -1;
    }

    known_host = true;
    return 0;
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

    ser_addr_size = sizeof(ser_addr);
    return 0;
}
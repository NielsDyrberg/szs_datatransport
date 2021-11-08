//
// Created by ncpd on 07-11-2021.
//

#ifndef SZS_DATATRANSPORT_UDP_SERVER_H
#define SZS_DATATRANSPORT_UDP_SERVER_H

#include "dataTransport.h"
#include <cstdint>
#include <arpa/inet.h>
#include <sys/socket.h>
#include<netinet/in.h>
#include <strings.h>

class UDP_server: public DataTransport{
private:
    bool port_open;  /* Keeps track on if the port is open */
    bool known_client;

    struct sockaddr_in /*ser_addr{},*/ cli_addr{};  /* Stores address data for the socket */
    struct sockaddr_storage /*ser_storage{},*/ cli_storage{};  /* - */
    socklen_t /*ser_addr_size,*/ cli_addr_size;  /* Size of the address data */

    int open_port();

public:
    explicit UDP_server(unsigned int port);

    int16_t receive(bool timeout = false);

    int send(uint8_t msg);
    int send(const uint8_t* msg, uint16_t msg_size);
    int send(const long long unsigned int *msg, uint8_t msg_size);
};


#endif //SZS_DATATRANSPORT_UDP_SERVER_H

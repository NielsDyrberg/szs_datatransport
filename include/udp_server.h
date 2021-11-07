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

class UDP_server{
private:
    unsigned int buffer_len;
    unsigned int port;  /* Port to use */
    bool port_open;  /* Keeps track on if the port is open */
    bool known_client;

    uint8_t buffer[BUFFER_LEN] = {0};  /* Buffer to store the  */
    int16_t bytes_recv = 0;

    struct sockaddr_in ser_addr{}, cli_addr{};  /* Stores address data for the socket */
    struct sockaddr_storage ser_storage{}, cli_storage{};  /* - */
    socklen_t ser_addr_size, cli_addr_size;  /* Size of the address data */

    int s;  /* Socket */

    int timeout_handler();
    int open_port();

public:
    UDP_server(unsigned int port);


    int16_t receive(bool timeout = false);

    /**
     * Sends #msg, where #msg is one byte.
     * @param msg[in] Bytes to send.
     * @returns int s.
     * @retval s=0 if msg sent, s<0 if send failed.
     */
    int send(uint8_t msg);

};


#endif //SZS_DATATRANSPORT_UDP_SERVER_H

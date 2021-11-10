//
// Created by ncpd on 07-11-2021.
//

#ifndef SZS_DATATRANSPORT_UDP_CLIENT_H
#define SZS_DATATRANSPORT_UDP_CLIENT_H

#include "dataTransport.h"
#include <cstdint>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <strings.h>


class UDP_client: public DataTransport{
private:
    bool known_host;  /* Keeps track on if the port is open */

    char* ser_hostname;  /* Hostname of the destination */
    char* ser_ip;  /* Ip of the destination */
    bool is_ip;  /* Defines if the program should use dst_hostname or dst_ip */

    struct sockaddr_in ser_addr{};  /* Stores address data for the socket */
    struct sockaddr_storage ser_storage{};  /* - */
    socklen_t ser_addr_size{};  /* Size of the address data */

    int setHost(char *host);
    int get_host_by_ip();

public:
    /**
     * Constructs a data-transport object.
     *
     * @param host[in] Hostname or ip of the device to where the connection should be made.
     * @param port[in] The port used for communication.
     * @param is_ip[in] Defines if the @host is ip or hostname.
     */
    UDP_client(char* host, unsigned int port, bool is_ip);
    UDP_client(char* host, unsigned int port, bool is_ip, uint8_t* buffer, uint16_t buffer_size);

    int16_t receive(bool timeout = false);
    int send(uint8_t msg);
    int send(const uint8_t* msg, uint16_t msg_size);
    int send(const long long unsigned int *msg, uint8_t msg_size);
    int16_t send_and_receive(uint8_t msg);
    int16_t send_and_receive(uint8_t *msg, uint16_t size);
};


#endif //SZS_DATATRANSPORT_UDP_CLIENT_H

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


class UDP_client{
private:
    unsigned int buffer_len;
    unsigned int port;  /* Port to use */
    bool connected;  /* Keeps track on if the port is open */

    char* ser_hostname;  /* Hostname of the destination */
    char* ser_ip;  /* Ip of the destination */
    bool is_ip;  /* Defines if the program should use dst_hostname or dst_ip */

    uint8_t buffer[BUFFER_LEN] = {0};  /* Buffer to store the  */
    int16_t bytes_recv = 0;

    struct sockaddr_in ser_addr{};  /* Stores address data for the socket */
    struct sockaddr_storage ser_storage{};  /* - */
    socklen_t ser_addr_size{};  /* Size of the address data */

    int s;  /* Socket */

    void setHost(char *host);
    int open_connection();
    int timeout_handler();
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

    int16_t receive(bool timeout = false);

    int send(uint8_t msg);

};


#endif //SZS_DATATRANSPORT_UDP_CLIENT_H

//
// Created by ncpd on 21-10-2021.
//

#ifndef C_SOUNDZONE_CLIENT_DATATRANSPORT_H
#define C_SOUNDZONE_CLIENT_DATATRANSPORT_H

#include <cstdint>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <strings.h>

#define ADDRESS_FAMILY AF_INET // Address is an ip type
#define SOCK_TYPE SOCK_DGRAM  // UDP


class DataTransport{
private:
    __time_t timeout_sec;
    __suseconds_t timeout_usec;

protected:
    int s;  /* socket */
    unsigned int port;  /* Port to use */

    unsigned int buffer_len;
    uint8_t* p_buffer;  /* Buffer to store the  */
    int16_t bytes_recv = 0;

    int listen_s;

    int timeout_handler();
    int set_listen_addr() const;

public:
    explicit DataTransport(unsigned int port);
    DataTransport(unsigned int port, uint8_t* buffer, uint16_t buffer_size);
    int16_t receive(bool timeout, struct sockaddr_in* addr= nullptr, socklen_t* addr_size= nullptr);

    int send(uint8_t msg, struct sockaddr_in* addr, const socklen_t* addr_size) const;
    int send(const uint8_t* msg, uint16_t msg_size, struct sockaddr_in* addr, const socklen_t* addr_size) const;
    int send(const long long unsigned int *msg, uint8_t msg_size, struct sockaddr_in* addr, const socklen_t* addr_size) const;

    uint8_t* GetBuffer(uint8_t* buff, uint16_t* size);
    long long unsigned int* GetBuffer(long long unsigned int* buff, uint8_t* size);
    uint16_t get_buffer();
    int set_timeout_len(unsigned int sec, unsigned int usec);
};

#endif //C_SOUNDZONE_CLIENT_DATATRANSPORT_H

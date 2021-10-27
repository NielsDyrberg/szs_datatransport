//
// Created by ncpd on 21-10-2021.
//

#ifndef C_SOUNDZONE_CLIENT_DATATRANSPORT_H
#define C_SOUNDZONE_CLIENT_DATATRANSPORT_H

#include <sys/socket.h>
#include <cstddef>
#include <cstdint>
#include <cstring>

#define PORT 1695 // port # for tcp
#define HEADER_SIZE 2 // two bytes for header size
#define ADDRESS_FAMILY AF_INET
#define SOCK_TYPE SOCK_DGRAM  // UDP
// #define ENCODING "utf-8"  // What encoding to use when encoding text
#define BUFFER_LEN 1028  // Length of rx buffer

class DataTransport{
private:
    unsigned int bufferLen;
    char* hostname;
    char* address;
    unsigned int port;
    unsigned int headerSize;
    // unsigned char encoding;
    unsigned char addressFamily; // Not sure on the type
    unsigned char sockType; // Not sure on the type
    int s; // socket
    bool portOpen;
    bool connected;
    uint8_t buffer[BUFFER_LEN] = {0};
    uint16_t bytes_recv = 0;

    int getHostByName();
    void open_port();
    void close();
    int connect();

public:
    DataTransport();
    explicit DataTransport(unsigned int port);
    DataTransport(unsigned int port, unsigned int buffer_len, unsigned int header_size=HEADER_SIZE,
                  unsigned char addr_family=ADDRESS_FAMILY, unsigned  char socket_type=SOCK_TYPE);
    unsigned char receive();
    void send(u_int8_t *msg, uint8_t size);
    char* GetHostname();
    uint8_t* GetBuffer(uint8_t* buff, uint16_t* size);
};

#endif //C_SOUNDZONE_CLIENT_DATATRANSPORT_H

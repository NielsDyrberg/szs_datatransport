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
#define BUFFER_LEN 1028  // Length of rx buffer


class DataTransport{
private:
    unsigned int buffer_len;
    char* dst_hostname;  /* Hostname of the destination */
    char* dst_ip;  /* Ip of the destination */
    unsigned int port;  /* Port to use */
    bool is_ip;  /* Defines if the program should use dst_hostname or dst_ip */

    struct sockaddr_in src_addr{}, dst_addr{};  /* Stores address data for the socket */
    struct sockaddr_storage src_storage{}, dst_storage{};  /* - */
    socklen_t src_addr_size, dst_addr_size;  /* Size of the address data */
    int src_s, dst_s;  /* Sockets */
    bool portOpen;  /* Keeps track on if the port is open */
    bool connected;  /* Keeps track on if a connection is established */
    uint8_t buffer[BUFFER_LEN] = {0};  /* Buffer to store the  */
    int16_t bytes_recv = 0;

    void setHost(char *host);
    int getHostByName();
    int getHostByIp();
    void open_port();

public:

    /**
     * Constructs a data-transport object.
     *
     * @param port[in] The port used for communication.
     */
    explicit DataTransport(unsigned int port);

    /**
     * Constructs a data-transport object.
     *
     * @param host[in] Hostname or ip of the device to where the connection should be made.
     * @param port[in] The port used for communication.
     * @param is_ip[in] Defines if the @host is ip or hostname.
     */
    DataTransport(char* host, unsigned int port, bool is_ip);

    /**
     * Opens a connection.
     *
     * @returns int s.
     * @retval s < 0 if connection could not be established, s > if socket was made.
     */
    int open_connection();
    void close();

    /**
     * Used to receive a msg.
     *
     * @return The size of received msg.
     */
    int16_t receive(bool timeout = false);

    /**
     * Sends #msg, where #msg is one byte.
     * @param msg[in] Bytes to send.
     * @returns int s.
     * @retval s=0 if msg sent, s<0 if send failed.
     */
    int send(uint8_t msg);

    /**
     * Sends #msg to the specified #hostname
     *
     * @param msg[in] Pointer to the msg to send.
     * @param size[in] Size of the msg to send.
     */
    int send(uint8_t *msg, uint16_t size);

    /**
     * Sends #msg to the specified #hostname
     *
     * @param msg[in] Pointer to the msg to send.
     * @param size[in] Size of the msg to send.
     */
    int send(long long unsigned int *msg, uint8_t size);

    int16_t send_and_receive(uint8_t msg);
    int16_t send_and_receive(uint8_t *msg, uint16_t size);

    /**
     * Used to get the received buffer.
     *
     * @param buff[in,out] Pointer to the msg buffer.
     * @return buff The pointer to the msg
     */
    uint8_t* GetBuffer(uint8_t* buff, uint16_t* size);

    /**
     * Used to get the received buffer.
     *
     * @param buff[in,out] Pointer to the msg buffer.
     * @return buff The pointer to the msg
     */
    long long unsigned int* GetBuffer(long long unsigned int* buff, uint8_t* size);
};

#endif //C_SOUNDZONE_CLIENT_DATATRANSPORT_H

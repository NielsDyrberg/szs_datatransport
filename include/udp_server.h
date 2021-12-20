/***********************************************************************************************************************
 * @name UDP_server
 * @file udp_server.h
 * @author Niels Dyrberg
 * @date 07-11-2021
 *
 * Purpose:
 *      Establish a udp server, so a udp client can connect.
 **********************************************************************************************************************/

#ifndef SZS_DATATRANSPORT_UDP_SERVER_H
#define SZS_DATATRANSPORT_UDP_SERVER_H

/**********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/

#include "dataTransport.h"
#include <cstdint>
#include <arpa/inet.h>
#include <sys/socket.h>
#include<netinet/in.h>
#include <strings.h>

/**********************************************************************************************************************
 * Class declaration
 **********************************************************************************************************************/

class UDP_server : public DataTransport {
private:
    bool port_open; /* Keeps track on if the port is open */
    bool known_client; /* Keeps track on if the client is known */

    struct sockaddr_in cli_addr{}; /* Stores address data for the socket */
    struct sockaddr_storage cli_storage{}; /* - */
    socklen_t cli_addr_size; /* Size of the address data */

    /**
     * @details Opens the port for listening.
     * @note !!Disclaimer!! This does not open the port anymore, but now just prepares #cli_addr for writting. But it is
     * still here to tie everything else together.
     * @return 0.
     */
    int open_port();

public:
    /**
     * @brief Constructor for UDP_server
     * @param type[in] DataTransport type.
     * @param port[in] UDP port to use.
     */
    explicit UDP_server(dt_type_t type, unsigned int port);

    /**
     * @brief Constructor for UDP_server
     * @param type[in] DataTransport type.
     * @param port[in] UDP port to use.
     * @param buffer[in] Pointer to external buffer.
     * @param buffer_size[in] Size of the buffer.
     */
    UDP_server(dt_type_t type, unsigned int port, uint8_t *buffer, uint16_t buffer_size);

    /**
     * @brief Receives data from the UDP port.
     * @param timeout[in] Decide if the receive function should timeout. If true it returns after timeout, if false it hangs.
     * @return int16_t.
     * @retval -1 if error.
     * @retval size of the received msg if successful.
     */
    int16_t receive(bool timeout = false);

    /**
     * @brief Sends #msg.
     * @param msg[in] Byte to send.
     * @return int
     * @retval -1 if errors
     * @retval The number of bytes sent if successfully.
     */
    int send(uint8_t msg);

    /**
     * @brief Sends #msg to #addr.
     * @param msg[in] Pointer to the array that should be sent.
     * @param msg_size[in] Size of #msg
     * @return int
     * @retval -1 if errors
     * @retval The number of bytes sent if successfully.
     */
    int send(const uint8_t *msg, uint16_t msg_size);

    /**
     * @brief Sends #msg to #addr.
     * @param msg[in] Pointer to the array that should be sent.
     * @param msg_size[in] Size of #msg IN BYTES!!
     * @return int
     * @retval -1 if errors
     * @retval The number of bytes sent if successfully.
     */
    int send(const long long unsigned int *msg, uint8_t msg_size);

    /**
     * @brief Sends #msg_size bytes from external buffer.
     * @param msg_size[in] Number of bytes to be sent.
     * @return int
     * @retval -1 if errors
     * @retval The number of bytes sent if successfully.
     */
    int send(uint16_t msg_size);
};


#endif //SZS_DATATRANSPORT_UDP_SERVER_H

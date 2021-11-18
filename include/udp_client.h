/***********************************************************************************************************************
 * @name UDP_client
 * @file udp_client.h
 * @author Niels Dyrberg
 * @date 07-11-2021
 *
 * Purpose:
 *      Establish a udp client, so it can connect to a udp server.
 **********************************************************************************************************************/

#ifndef SZS_DATATRANSPORT_UDP_CLIENT_H
#define SZS_DATATRANSPORT_UDP_CLIENT_H

/**********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/

#include "dataTransport.h"
#include <cstdint>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <strings.h>

/**********************************************************************************************************************
 * Class declaration
 **********************************************************************************************************************/


class UDP_client : public DataTransport {
private:
    bool known_host;  /* Keeps track on if the port is open */

    char *ser_hostname;  /* Hostname of the destination */
    char *ser_ip;  /* Ip of the destination */
    bool is_ip;  /* Defines if the program should use dst_hostname or dst_ip */

    struct sockaddr_in ser_addr{};  /* Stores address data for the socket */
    struct sockaddr_storage ser_storage{};  /* - */
    socklen_t ser_addr_size{};  /* Size of the address data */

    int setHost(char *host);

    int get_host_by_ip();

public:
    /**
     * @brief Default constructor for UDP_client
     * @details Everything is set to zero or nullptr. So it cant be used. It is only a placeholder
     */
    UDP_client();

    /**
     * @brief Constructor for UDP_client
     * @param type[in] DataTransport type.
     * @param host[in] Ip or hostname of the server.
     * @param port[in] UDP port to use.
     * @param is_ip[in] Is #host an ip.
     */
    UDP_client(dt_type_t type, char *host, unsigned int port, bool is_ip);

    /**
     * @brief Constructor for UDP_client
     * @param type[in] DataTransport type.
     * @param host[in] Ip or hostname of the server.
     * @param port[in] UDP port to use.
     * @param is_ip[in] Is #host an ip.
     * @param buffer[in]  Pointer to external buffer.
     * @param buffer_size[in] Size of the buffer.
     */
    UDP_client(dt_type_t type, char *host, unsigned int port, bool is_ip, uint8_t *buffer, uint16_t buffer_size);


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

    /**
     * @brief Sends #msg and then wait until #timeout for response.
     * @param msg The byte to send.
     * @return int
     * @retval if int > 0, successful and is the number of bytes received.
     * @retval -1 if send failed.
     * @retval -2 if receive failed.
     */
    int16_t send_and_receive(uint8_t msg);

    /**
     * @brief Sends #msg and then wait until #timeout for response.
     * @param msg Pointer to the message to send.
     * @param size Size of #msg.
     * @return int
     * @retval if int > 0, successful and is the number of bytes received.
     * @retval -1 if send failed.
     * @retval -2 if receive failed.
     */
    int16_t send_and_receive(uint8_t *msg, uint16_t size);

    /**
     * @brief Sends msg from external buffer, and then wait until #timeout for response.
     * @param Size of msg to send.
     * @return int
     * @retval if int > 0, successful and is the number of bytes received.
     * @retval -1 if send failed.
     * @retval -2 if receive failed.
     */
    int16_t send_and_receive(uint16_t msg_size);
};


#endif //SZS_DATATRANSPORT_UDP_CLIENT_H

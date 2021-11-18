/***********************************************************************************************************************
 * Name: DataTransport
 *
 * File: dataTransport.h
 *
 * Author:
 *      Niels Dyrberg
 *
 * Date:
 *      21-10-2021
 *
 * Purpose:
 *      Transport an array of bytes from one machine to another.
 *
 * Notes:
 *      Is written as a base-class for UDP_server and UDP_client.
 **********************************************************************************************************************/

#ifndef C_SOUNDZONE_CLIENT_DATATRANSPORT_H
#define C_SOUNDZONE_CLIENT_DATATRANSPORT_H

/**********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/

#include <cstdint>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <strings.h>

/**********************************************************************************************************************
 * Defines
 **********************************************************************************************************************/

#define ADDRESS_FAMILY AF_INET // Address is an ip type
#define SOCK_TYPE SOCK_DGRAM  // UDP

/**********************************************************************************************************************
 * Typedef's
 **********************************************************************************************************************/

typedef enum {
    SZP = 1,
    SYNC = 2
} dt_type_t;

class DataTransport {
private:
    __time_t timeout_sec;
    __suseconds_t timeout_usec;

protected:
    int *listen_s; /* Pointer to the socket used for listening used */
    bool *listen_socket_set; /* Poionter to the bool teeling if the listen port is in use */
    static int listen_s_1; /* Socket used to incomming traffic 1 */
    static bool listen_socket_set_1; /* Keeping track on if listen_socket_1 i set */
    static int listen_s_2; /* Socket used to incomming traffic 2 */
    static bool listen_socket_set_2; /* Keeping track on if listen_socket_2 i set */

    int s;  /* socket */
    unsigned int port;  /* Port to use */

    unsigned int buffer_len;
    uint8_t *p_buffer;  /* Buffer to store the  */
    int16_t bytes_recv = 0;

    int timeout_handler();
    int set_listen_addr() const;

public:
    /**
     * @brief Default constructor for DataTransport
     * @details Everything is set to zero or nullptr. So it cant be used. It is only a placeholder
     */
    DataTransport();

    /**
     * @brief Constructor for DataTransport
     * @param type Is it for SZP of SYNC.
     * @param port The port to use.
     */
    explicit DataTransport(dt_type_t type, unsigned int port);

    /**
     * @brief Constructor for DataTransport
     * @details When using this an external buffer is used. This eases up the process of retrieving the data from
     * DataTransport.
     * @param type Is it for SZP of SYNC.
     * @param port The port to use.
     * @param buffer Pointer to an external buffer.
     * @param buffer_size The size of #buffer.
     */
    DataTransport(dt_type_t type, unsigned int port, uint8_t *buffer, uint16_t buffer_size);

    /**
     * @brief Receives data and saves the incoming ip.
     * @param timeout Decide if the receive function should timeout. If true it returns after timeout, if false it hangs.
     * @param addr The address of the sender.
     * @param addr_size The size of #addr.
     * @return int16_t.
     * @retval -1 if error.
     * @retval size of the received msg if successful.
     */
    int16_t receive(bool timeout, struct sockaddr_in *addr = nullptr, socklen_t *addr_size = nullptr);

    /**
     * @brief Sends #msg to #addr.
     * @param msg Byte to send.
     * @param addr The address of the receiver.
     * @param addr_size The size of #addr.
     * @return int
     * @retval -1 if errors
     * @retval The number of bytes sent if successfully.
     */
    int send(uint8_t msg, struct sockaddr_in *addr, const socklen_t *addr_size) const;

    /**
     * @brief Sends #msg to #addr.
     * @param msg Pointer to the array that should be sent.
     * @param msg_size Size of #msg
     * @param addr The address of the receiver.
     * @param addr_size The size of #addr.
     * @return int
     * @retval -1 if errors
     * @retval The number of bytes sent if successfully.
     */
    int send(const uint8_t *msg, uint16_t msg_size, struct sockaddr_in *addr, const socklen_t *addr_size) const;

    /**
     * @brief Sends #msg to #addr.
     * @param msg Pointer to the array that should be sent.
     * @param msg_size Size of #msg IN BYTES!!
     * @param addr The address of the receiver.
     * @param addr_size The size of #addr.
     * @return int
     * @retval -1 if errors
     * @retval The number of bytes sent if successfully.
     */
    int send(const long long unsigned int *msg, uint8_t msg_size, struct sockaddr_in *addr,
             const socklen_t *addr_size) const;

    /**
     * @brief Used to get pointers to the communication buffer.
     * @details If an external buffer is used, consider using get_buffer().
     * @param buff Pointer to the communication buffer.
     * @param size Will carry the number of bytes received.
     * @return Pointer to the communication buffer.
     */
    uint8_t *GetBuffer(uint8_t *buff, uint16_t *size);

    /**
     * @brief Used to get pointers to the communication buffer.
     * @param buff Pointer to the communication buffer.
     * @param size Will carry the number of bytes received.
     * @return Pointer to the communication buffer.
     */
    long long unsigned int *GetBuffer(long long unsigned int *buff, uint8_t *size);

    /**
     * Getter for #bytes_recv.
     * @return #bytes_recv.
     */
    uint16_t get_buffer();

    /**
     * Setter for the timeout lengths.
     * @details The two values are added (sec + usec*10^-6).
     * @param sec Seconds to timeout.
     * @param usec Micro-seconds to timeout.
     * @return int
     * @retval -1 If error
     * @retval 0 If successful update.
     */
    int set_timeout_len(unsigned int sec, unsigned int usec);
};

#endif //C_SOUNDZONE_CLIENT_DATATRANSPORT_H

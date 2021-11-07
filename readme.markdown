<!-- 
To compile puml use: (Assuming plantuml you are in the directory)
plantuml.jar -tsvg readme.md -o diagrams
-->

A python module mainly building on sockets. 
It is used to transport data between a server and clients, 
for the SoundZone Protocol.
In the layer topology this module is between layer 7 (SZP) and layer 4

## How to use

This section will give a few examples on how to use the class.

### Create socket

```Python
hostname = "hostname_of_device_communicating"
comm_socket = DataTransport(hostname)  # Creates socket from where to communicate
```

### Recieve

```Python
hostname = "hostname_of_the_sender"
receive_socket = DataTransport(hostname)  # Creates socket to receive from

rcv_payload, client_ip = receive_socket.receive(return_sender_addr=True)  # Returns the recieved message (and client_ip if return_sender_addr is true)
```

### Transmit

```python
hostname = "hostname_of_the_reciever"
send_socket = DataTransport(hostname)

msg_to_send = b'\x83'  # Some hex encoded string
send_socket.send(msg_to_send)
```


## Class diagram

<!--
```
@startuml class_diagram

DataTransport --* socket

class DataTransport {
    + buffer_len: int
    - _ADDR: str
    - _PORT: int
    - _HEADER_SIZE: int
    - _ENCODING: str
    - _ADDRESS_FAMILY: socket.AddressFamily
    - _SOCK_TYPE: socket.SocketKind
    - _s: socket object
    - _sender_socket:
    - _port_open: bool
    - _connected: bool

    + receive(return_sender_addr)
    + send(msg):
    - _open_port()
    - _close()
    - _connect()
}

class socket {
    + AF_INET
    + SOCK_DGRAM

    + socket(family, type)
    + bind((address, port))
    + recvfrom(buffer_size: int)
    + send(msg: bytes)
    + connect()
    + close()
}

note right of socket::SOCK_DGRAM
    UDP
end note


@enduml
```
-->

![](diagrams/class_diagram.svg)

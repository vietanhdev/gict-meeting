// The ServerSocket class provides an abstraction for a UDP socket that
// listens to incoming packets from the sender on a specified port number.
//
// Pass the desired port number to listen on into the constructor. Then call
// BindSocketToListen() to bind the socket. If that method returns true, you can
// call GetPacket() to get incoming packets from the network.

#ifndef SERVER_SOCKET_H
#define SERVER_SOCKET_H

#include <vector>
#include "packet.h"

#include <arpa/inet.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h> 

#include <iostream>
#include <vector>
#include "packet.h"

#include "cipher.h"

#include "conference.h"

// This is the maximum UDP packet size, and the buffer will be allocated for
// the max amount.
constexpr int kMaxPacketBufferSize = 65535;

class ServerSocket {
   public:
    // Creates a new socket and stores the handle.
    explicit ServerSocket(const int port_number);

    int getSockFd();

    // Attempts to bind the socket on the port number provided to listen to
    // incoming UDP packets. If the socket could not be created (in the
    // constructor) or if the binding process fails, an error message will be
    // printed to stderr. The method returns true on success, false otherwise.
    const bool bindSocketToListen() const;

    // Waits for the next packet on the given port, and returns a packet: client address and data as a vector of bytes
    const Packet getPacket() const;

    // Send package to client
    int sendPackage(sockaddr_in receiver_addr_, std::vector<unsigned char> data);

   private:
    // This buffer will be used to collect incoming packet data. It is only used
    // in the GetPacket() method.
    char buffer_[kMaxPacketBufferSize];

    // The port number that the socket will listen for packets on.
    const int port_;

    // The socket identifier (handle).
    int socket_handle_;

    // Key for connection encryption
    std::string secret_key;

};  // ServerSocket

#endif  // server_socket_H

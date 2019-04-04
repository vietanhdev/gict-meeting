#ifndef CLIENT_SOCKET_H
#define CLIENT_SOCKET_H

#include <arpa/inet.h>

#include <string>
#include <vector>


// This is the maximum UDP packet size, and the buffer will be allocated for
// the max amount.
constexpr int kMaxPacketBufferSize = 65535;

class ClientSocket {

   public:
    ClientSocket();
    void init(const std::string &receiver_ip, const int receiver_port);
    void destroy();
    void sendPacket(const std::vector<unsigned char> &data) const;
    std::vector<unsigned char> getPacket() const;

   private:
    // The socket identifier (handle).
    int socket_handle_;

    // The struct that contains the receiver's address and port. This is set up
    // in the constructor.
    sockaddr_in server_addr_;

    // This buffer will be used to collect incoming packet data. It is only used
    // in the GetPacket() method.
    char buffer_[kMaxPacketBufferSize];

};  // ClientSocket

#endif  // client_socket_H

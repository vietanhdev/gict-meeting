#ifndef SENDER_SOCKET_H
#define SENDER_SOCKET_H

#include <arpa/inet.h>

#include <string>
#include <vector>


class SenderSocket {

   public:
    SenderSocket();
    void init(const std::string &receiver_ip, const int receiver_port);
    void destroy();
    void sendPacket(const std::vector<unsigned char> &data) const;

   private:
    // The socket identifier (handle).
    int socket_handle_;

    // The struct that contains the receiver's address and port. This is set up
    // in the constructor.
    sockaddr_in receiver_addr_;

};  // SenderSocket

#endif  // SENDER_SOCKET_H

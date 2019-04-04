#ifndef CLIENT_SOCKET_H
#define CLIENT_SOCKET_H

#include <arpa/inet.h>

#include <string>
#include <vector>


class ClientSocket {

   public:
    ClientSocket();
    void init(const std::string &receiver_ip, const int receiver_port);
    void destroy();
    void sendPacket(const std::vector<unsigned char> &data) const;

   private:
    // The socket identifier (handle).
    int socket_handle_;

    // The struct that contains the receiver's address and port. This is set up
    // in the constructor.
    sockaddr_in receiver_addr_;

};  // ClientSocket

#endif  // client_socket_H

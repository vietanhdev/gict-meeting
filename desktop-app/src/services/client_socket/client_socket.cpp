#include "client_socket.h"

#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#include <iostream>
#include <string>
#include <vector>

ClientSocket::ClientSocket() {}

void ClientSocket::init(const std::string &receiver_ip, const int receiver_port) {
    socket_handle_ = socket(AF_INET, SOCK_DGRAM, 0);
    receiver_addr_.sin_family = AF_INET;
    receiver_addr_.sin_port = htons(receiver_port);
    receiver_addr_.sin_addr.s_addr = inet_addr(receiver_ip.c_str());
}

void ClientSocket::destroy() {
    close(socket_handle_);
}

void ClientSocket::sendPacket(const std::vector<unsigned char> &data) const {
    sendto(socket_handle_, data.data(), data.size(), 0,
           const_cast<sockaddr *>(
               reinterpret_cast<const sockaddr *>(&receiver_addr_)),
           sizeof(receiver_addr_));
}

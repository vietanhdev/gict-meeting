#include "client_socket.h"

#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/types.h> 

#include <iostream>
#include <string>
#include <vector>

ClientSocket::ClientSocket() {}

void ClientSocket::init(const std::string &receiver_ip, const int receiver_port) {
    socket_handle_ = socket(AF_INET, SOCK_DGRAM, 0);
    server_addr_.sin_family = AF_INET;
    server_addr_.sin_port = htons(receiver_port);
    server_addr_.sin_addr.s_addr = inet_addr(receiver_ip.c_str());

    struct timeval read_timeout;
    read_timeout.tv_sec = 0;
    read_timeout.tv_usec = 10;
    setsockopt(socket_handle_, SOL_SOCKET, SO_RCVTIMEO, &read_timeout, sizeof read_timeout);
}

void ClientSocket::destroy() {
    close(socket_handle_);
}

void ClientSocket::sendPacket(const std::vector<unsigned char> &data) const {
    sendto(socket_handle_, data.data(), data.size(), 0,
           const_cast<sockaddr *>(
               reinterpret_cast<const sockaddr *>(&server_addr_)),
           sizeof(server_addr_));
}


std::vector<unsigned char> ClientSocket::getPacket() const {

    std::vector<unsigned char> data;
    
    // Get the data from the next incoming packet.
    socklen_t addrlen = sizeof(server_addr_);
    const int num_bytes = recvfrom(socket_handle_, (char *)buffer_, kMaxPacketBufferSize,  
                MSG_WAITALL, (struct sockaddr *) &server_addr_, 
                &addrlen);
            
    // Copy the data (if any) into the data vector.
    if (num_bytes > 0) {
        data.insert(data.end(), &buffer_[0], &buffer_[num_bytes]);
    }

    return data;
}
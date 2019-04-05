#include "client_socket.h"

#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/types.h> 

#include <iostream>
#include <string>
#include <vector>

#include "errno.h"

ClientSocket::ClientSocket() {
    secret_key = Conference::instance().getSecretKey();
}

void ClientSocket::init(const std::string &receiver_ip, const int receiver_port) {
    socket_handle_ = socket(AF_INET, SOCK_DGRAM, 0);
    server_addr_.sin_family = AF_INET;
    server_addr_.sin_port = htons(receiver_port);
    server_addr_.sin_addr.s_addr = inet_addr(receiver_ip.c_str());

    struct timeval read_timeout;
    read_timeout.tv_sec = 0;
    read_timeout.tv_usec = 10;
    setsockopt(socket_handle_, SOL_SOCKET, SO_RCVTIMEO, &read_timeout, sizeof read_timeout);

    this->receiver_ip = receiver_ip;
    this->receiver_port = receiver_port;
}

void ClientSocket::destroy() {
    close(socket_handle_);
}

void ClientSocket::sendPacket(std::vector<unsigned char> data) {

    // Ecrypt the packet
    Cipher::xor_crypt(secret_key, data);

    int result = sendto(socket_handle_, data.data(), data.size(), 0,
           const_cast<sockaddr *>(
               reinterpret_cast<const sockaddr *>(&server_addr_)),
           sizeof(server_addr_));


    if (result == -1) {
        switch(errno){
            case EFAULT:
                std::cout << "Invalid user space address.\n" << std::endl;
                break;
            case EBADF:
                std::cout << "Invalid descriptor.\n" << std::endl;
                break;
            case EINVAL:
                std::cout << "Invalid argument.\n" << std::endl;
                break;
            case EDESTADDRREQ:
                std::cout << "No Peer address.\n" << std::endl;
                break;
            case EISCONN:
                std::cout << "Connection mode socket.\n" << std::endl;
                break;
            case ENOTSOCK:
                std::cout << "The given socket is not a socket.\n" << std::endl;
                break;
            case ENOTCONN:
                std::cout << "No Target.\n" << std::endl;
                break;
            case ENOBUFS:
                std::cout << "Network output is full.\n" << std::endl;
                break;
            default:
                std::cout << "No spezific error.\n" << std::endl;
        }



        // Try to re-init the socket and send again
        init(receiver_ip, receiver_port);
        int result = sendto(socket_handle_, data.data(), data.size(), 0,
           const_cast<sockaddr *>(
               reinterpret_cast<const sockaddr *>(&server_addr_)),
           sizeof(server_addr_));

        if (result == -1) {
            std::cout << "Error on sending packet" << std::endl;
        }
    }
    
    
    
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

    // Decrypt the packet
    Cipher::xor_crypt(secret_key, data);

    return data;
}
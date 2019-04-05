#include "server_socket.h"


ServerSocket::ServerSocket(const int port_number) : port_(port_number) {
    socket_handle_ = socket(AF_INET, SOCK_DGRAM, 0);
    struct timeval read_timeout;
    read_timeout.tv_sec = 0;
    read_timeout.tv_usec = 10;
    setsockopt(socket_handle_, SOL_SOCKET, SO_RCVTIMEO, &read_timeout, sizeof read_timeout);

    secret_key = Conference::instance().getSecretKey();
}

const bool ServerSocket::bindSocketToListen() const {
    if (socket_handle_ < 0) {
        std::cerr << "Binding failed. Socket was not initialized." << std::endl;
        return false;
    }
    // Bind socket's address to INADDR_ANY because it's only receiving data, and
    // does not need a valid address.
    sockaddr_in socket_addr;
    memset(reinterpret_cast<char*>(&socket_addr), 0, sizeof(socket_addr));
    socket_addr.sin_family = AF_INET;
    socket_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    // Bind the socket to the port that will be listened on.
    socket_addr.sin_port = htons(port_);
    const int bind_status =
        bind(socket_handle_, reinterpret_cast<sockaddr*>(&socket_addr),
             sizeof(socket_addr));
    if (bind_status < 0) {
        std::cerr << "Binding failed. Could not bind the socket." << std::endl;
        return false;
    }
    return true;
}

int ServerSocket::getSockFd() {
    return socket_handle_;
}

const Packet ServerSocket::getPacket() const {

    Packet packet;
    
    // Get the data from the next incoming packet.
    socklen_t addrlen = sizeof(packet.client_addr);
    const int num_bytes =
        recvfrom(socket_handle_,
                 const_cast<void*>(reinterpret_cast<const void*>(buffer_)),
                 kMaxPacketBufferSize, 0,
                 reinterpret_cast<sockaddr*>(&packet.client_addr), &addrlen);
    // Copy the data (if any) into the data vector.
    if (num_bytes > 0) {
        packet.data.insert(packet.data.end(), &buffer_[0], &buffer_[num_bytes]);
    }

    // Decrypt the packet
    Cipher::xor_crypt(secret_key, packet.data);

    return packet;
}


int ServerSocket::sendPackage(sockaddr_in receiver_addr_, std::vector<unsigned char> data) {

    // Ecrypt the packet
    Cipher::xor_crypt(secret_key, data);

    return sendto(socket_handle_, data.data(), data.size(), 0,
        const_cast<sockaddr *>(
            reinterpret_cast<const sockaddr *>(&receiver_addr_)),
        sizeof(receiver_addr_));

}

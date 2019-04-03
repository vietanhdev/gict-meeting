#ifndef PACKET_H
#define PACKET_H

#include <vector>
#include <arpa/inet.h>

class Packet {
    public:
        sockaddr_in client_addr;
        std::vector<unsigned char> data;
};

#endif
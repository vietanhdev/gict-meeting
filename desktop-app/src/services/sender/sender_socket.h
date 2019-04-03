#ifndef SENDER_SOCKET_H
#define SENDER_SOCKET_H

#include <arpa/inet.h>

#include <string>
#include <vector>

namespace udp_streaming_video {

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

}  // namespace udp_streaming_video

#endif  // SENDER_SOCKET_H

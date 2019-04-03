// Streaming Server
#include <iostream>
#include <vector>

#include "basic_protocol.h"
#include "receiver_socket.h"

int main(int argc, char** argv) {
    const int port = 8881;
    if (port < 0) {
        return -1;
    }
    const ReceiverSocket socket(port);
    if (!socket.BindSocketToListen()) {
        std::cerr << "Could not bind socket." << std::endl;
        return -1;
    }
    std::cout << "Listening on port " << port << "." << std::endl;
    BasicProtocolData protocol_data;
    while (true) {  // TODO: break out cleanly when done.
        protocol_data.UnpackData(socket.GetPacket());
        protocol_data.GetImage().Display();
    }
    return 0;
}

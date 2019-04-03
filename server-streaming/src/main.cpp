// Streaming Server
#include <iostream>
#include <vector>
#include <cstdlib>

#include "conference.h"
#include "basic_protocol.h"
#include "receiver_socket.h"

int main(int argc, char** argv) {
    // Welcome message
    std::cout << "iCT VIDEO CONFERENCE\n" << "Streaming Server" << "\n";

    if (argc != 2) {
        std::cerr << "Wrong input parameters. Syntax: " << argv[0] << " <config file>" << std::endl;
    }

    std::string config_file(argv[1]);

    // Read the configuration
    Conference &conference = Conference::instance();
    conference.readFromFile(config_file);


    const int port = std::atoi (conference.video_up_port.c_str());
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

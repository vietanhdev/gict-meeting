// Streaming Server
#include <iostream>
#include <vector>
#include <cstdlib>
#include <thread>

#include "conference.h"
#include "server_socket.h"
#include "video_stream_service.h"

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

    // Start threads
    std::thread video_up(VideoStreamService::videoUpService);
    std::thread video_down_listen(VideoStreamService::videoDownServiceListening);
    std::thread video_down_send(VideoStreamService::videoDownServiceSending);

    video_up.join();
    video_down_listen.join();
    video_down_send.join();

    return 0;
}

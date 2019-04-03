
#include "video_stream_service.h"

VideoStreamService::VideoStreamService() {}

void VideoStreamService::videoUpService() {
    Conference &conference = Conference::instance();
    const int port = conference.getVideoUpPort();
    const ReceiverSocket socket(port);
    if (!socket.bindSocketToListen()) {
        std::cerr << "Could not bind socket." << std::endl;
        return;
    }
    std::cout << "Listening on port " << port << "." << std::endl;
    BasicProtocolData protocol_data;
    while (true) {
        protocol_data.unpackData(socket.getPacket());
        protocol_data.getImage().display();
    }
}

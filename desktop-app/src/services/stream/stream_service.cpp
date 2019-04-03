
#include "stream_service.h"

using udp_streaming_video::BasicProtocolData;
using udp_streaming_video::SenderSocket;
using udp_streaming_video::VideoCapture;

StreamService::StreamService() {}

void StreamService::upStreamingThread() {

    SenderSocket socket;
    VideoCapture video_capture(false, 10);
    BasicProtocolData protocol_data;

    for (;;) {
        StreamService &streaming_service = StreamService::instance();
        bool streaming = streaming_service.isStreaming();
        bool new_streaming_status = streaming_service.streaming_signal_flag;
        if (new_streaming_status != streaming) {
            if (new_streaming_status == true) { // Start streaming

                // Get up port
                std::istringstream iss (streaming_service.conference.video_up_port);
                int port;
                iss >> port;

                // Server ip addresss
                std::string ip_address = streaming_service.conference.server_ip;

                // Init a socket to the server
                socket.init(ip_address, port);

            } else { // Stop streaming

                // Destroy socket connection to the server
                socket.destroy();

            }

            streaming_service.streaming = new_streaming_status;
        }

        if (new_streaming_status) {
            protocol_data.SetImage(video_capture.GetFrameFromCamera());
            socket.sendPacket(protocol_data.PackageData());
        }

    }
}


void StreamService::downStreamingThread() {

    for (;;) {

    }
}

bool StreamService::isStreaming() { return this->streaming; }

void StreamService::setStreaming(bool streaming) {
    this->streaming_signal_flag = streaming;
}

bool StreamService::startStreaming() {
    if (StreamService::instance().isStreaming()) {
        return false;
    }

    // Load current conference config
    this->conference = Conference::instance();

    if (streaming_thread == nullptr) {
        // Start streaming
        up_streaming_thread = std::make_shared<std::thread>(upStreamingThread);
        down_streaming_thread = std::make_shared<std::thread>(downStreamingThread);
    }

    StreamService::instance().setStreaming(true);

    return true;
}

bool StreamService::stopStreaming() {
    if (!StreamService::instance().isStreaming()) {
        return false;
    }

    StreamService::instance().setStreaming(false);
    return true;
}
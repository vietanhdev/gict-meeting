
#include "stream_service.h"

StreamService::StreamService() {}

void StreamService::upStreamingThread() {

    SenderSocket socket;
    VideoCapture video_capture(false, 0.5);
    BasicProtocolData protocol_data;

    for (;;) {
        StreamService &streaming_service = StreamService::instance();
        bool streaming = streaming_service.isStreaming();
        bool new_streaming_status = streaming_service.streaming_signal_flag;
        if (new_streaming_status != streaming) {
            if (new_streaming_status == true) { // Start streaming

                // Get up port
                int port = streaming_service.conference.getVideoUpPort();

                // Server ip addresss
                std::string ip_address = streaming_service.conference.getServerIp();

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

    if (up_streaming_thread == nullptr) {
        up_streaming_thread = std::make_shared<std::thread>(upStreamingThread);
    }

    if (down_streaming_thread == nullptr) {
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
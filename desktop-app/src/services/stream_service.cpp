
#include "stream_service.h"


StreamService::StreamService() {}

void StreamService::streamingThread() {
    for (;;) {
        if (StreamService::instance().isStreaming()) {

        } 
    }
}

bool StreamService::isStreaming() {
    return this->streaming;
}

void StreamService::setStreaming(bool streaming) {
    this->streaming = streaming;
}

bool StreamService::startStreaming() {

    if (StreamService::instance().isStreaming()) {
        return false;
    }

    // Load current conference config
    this->conference = Conference::instance();

    if (streaming_thread == nullptr) {
        // Start streaming
        streaming_thread = std::make_shared<std::thread>(streamingThread);
    }

    StreamService::instance().setStreaming(true);

    std::cout << "START STREAMING" << std::endl;

    return true;
}


bool StreamService::stopStreaming() {

    if (!StreamService::instance().isStreaming()) {
        return false;
    }

    StreamService::instance().setStreaming(false);

    std::cout << "STOP STREAMING" << std::endl;

    return true;
}
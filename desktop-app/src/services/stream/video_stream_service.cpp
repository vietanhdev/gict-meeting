
#include "video_stream_service.h"

VideoStreamService::VideoStreamService() {}

void VideoStreamService::upStreamingThread() {

    ClientSocket socket;
    VideoCapture video_capture(false, 0.5);
    VideoFrameProtocolData protocol_data;

    for (;;) {
        VideoStreamService &streaming_service = VideoStreamService::instance();
        bool streaming = streaming_service.isStreaming();
        bool new_streaming_status = streaming_service.streaming_signal_flag;
        if (new_streaming_status != streaming) {
            if (new_streaming_status == true) { // Start streaming

                // Get up port
                int port = Conference::instance().getVideoUpPort();

                // Server ip addresss
                std::string ip_address = Conference::instance().getServerIp();

                // Init a socket to the server
                socket.init(ip_address, port);

            } else { // Stop streaming

                // Destroy socket connection to the server
                socket.destroy();

            }

            streaming_service.streaming = new_streaming_status;
        }

        if (new_streaming_status) {
            cv::Mat image = video_capture.getFrameFromCamera().getImage();
            socket.sendPacket(protocol_data.packageClientFrame(image));
        }

    }
}


void VideoStreamService::downStreamingThread() {

    ClientSocket socket;
    VideoFrameProtocolData protocol_data;
    for (;;) {
        VideoStreamService &streaming_service = VideoStreamService::instance();
        bool streaming = streaming_service.isStreaming();
        bool new_streaming_status = streaming_service.streaming_signal_flag;
        if (new_streaming_status != streaming) {
            if (new_streaming_status == true) { // Start streaming

                // Get up port
                int port = Conference::instance().getVideoDownPort();

                // Server ip addresss
                std::string ip_address = Conference::instance().getServerIp();

                // Init a socket to the server
                socket.init(ip_address, port);

                // Send stream request message
                socket.sendPacket(protocol_data.packageStreamRequestMessage());
                

                std::cout << "Requesting video streaming from server" << std::endl;

            } else { // Stop streaming

                // Destroy socket connection to the server
                socket.destroy();

            }

            streaming_service.streaming = new_streaming_status;
        }

        //Streaming
        if (new_streaming_status) {
            std::vector<unsigned char> data = socket.getPacket();

            if (data.empty()) {
                continue;
            }

            cv::Mat img = protocol_data.unpackConferenceFrame(data);

            if (img.empty()) {
                std::cout << "Could not parse stream from server." << std::endl;
                continue;
            }

            Conference::instance().setConferenceFrame(img);

            emit VideoStreamService::instance().newConferenceFrame();
        }

    }
}

bool VideoStreamService::isStreaming() { return this->streaming; }

void VideoStreamService::setStreaming(bool streaming) {
    this->streaming_signal_flag = streaming;
}

bool VideoStreamService::startStreaming() {
    if (VideoStreamService::instance().isStreaming()) {
        return false;
    }

    if (up_streaming_thread == nullptr) {
        up_streaming_thread = std::make_shared<std::thread>(upStreamingThread);
    }

    if (down_streaming_thread == nullptr) {
        down_streaming_thread = std::make_shared<std::thread>(downStreamingThread);
    }

    VideoStreamService::instance().setStreaming(true);

    return true;
}

bool VideoStreamService::stopStreaming() {
    if (!VideoStreamService::instance().isStreaming()) {
        return false;
    }

    VideoStreamService::instance().setStreaming(false);
    return true;
}
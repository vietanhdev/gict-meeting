
#include "video_stream_service.h"

VideoStreamService::VideoStreamService() {}


// Set camera path (maybe a http URL)
void VideoStreamService::setCameraPath(const std::string& path) {
    this->use_camera_path = true;
    this->camera_path = path;
}

void VideoStreamService::upStreamingThread() {

    ClientSocket socket;
    std::shared_ptr<VideoCapture> video_capture;

    std::cout << "Use camera path: " << VideoStreamService::instance().use_camera_path << std::endl;

    if (VideoStreamService::instance().use_camera_path) {
        std::cout << "Setting up camera path: " << VideoStreamService::instance().camera_path << std::endl;
        video_capture = std::make_shared<VideoCapture>(false, cv::Size(640, 480), VideoStreamService::instance().camera_path);
    } else {
        video_capture = std::make_shared<VideoCapture>(false, cv::Size(640, 480), -1);
    }


    VideoFrameProtocolData protocol_data;

    // Get up port
    int port = Conference::instance().getVideoUpPort();
    // Server ip addresss
    std::string ip_address = Conference::instance().getServerIp();
    // Init a socket to the server
    socket.init(ip_address, port);

    for (;;) {
        VideoStreamService &stream_service = VideoStreamService::instance();
        bool streaming = stream_service.isStreaming();
        bool new_streaming_status = stream_service.streaming_signal_flag;
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

            stream_service.streaming = new_streaming_status;
        }

        if (new_streaming_status) {
            cv::Mat image = video_capture->getFrameFromCamera().getImage();
            if (image.empty()) {
                // std::cerr << "Could not get image from camera" << std::endl;
                continue;
            }

            // Flip frame if needed
            if (stream_service.shouldFlipCamFrame()) {
                cv::flip(image, image, 1);
            }

            // Check time to ensure keep sending in prefered fps
            if (Timer::calcTimePassed(stream_service.last_image_up_time) > 1000 / stream_service.frame_up_fps ) {
                socket.sendPacket(protocol_data.packageClientFrame(image));
                stream_service.last_image_up_time = Timer::getCurrentTime();
            }

            Conference::instance().setClientCamFrame(image);
            emit VideoStreamService::instance().newClientCamFrame();
        }

    }
}


void VideoStreamService::downStreamingThread() {

    ClientSocket socket;
    VideoFrameProtocolData protocol_data;
    for (;;) {
        VideoStreamService &stream_service = VideoStreamService::instance();
        bool streaming = stream_service.isStreaming();
        bool new_streaming_status = stream_service.streaming_signal_flag;
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

            stream_service.streaming = new_streaming_status;
        }

        //Streaming
        if (new_streaming_status) {
            std::vector<unsigned char> data = socket.getPacket();

            if (data.empty()) {
                continue;
            }

            cv::Mat img = protocol_data.unpackConferenceFrame(data);

            if (img.empty()) {
                // std::cout << "Could not parse stream from server." << std::endl;
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


// Set to flip camera frame or not
void VideoStreamService::setFlipCamFrame(bool flip) {
    flip_cam = flip;
}
bool VideoStreamService::shouldFlipCamFrame() {
    return flip_cam;
}

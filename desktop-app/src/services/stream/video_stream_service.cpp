
#include "video_stream_service.h"

VideoStreamService::VideoStreamService() {}


// Set camera path (maybe a http URL)
void VideoStreamService::setCameraPath(const std::string& path) {
    this->use_camera_path = true;
    this->camera_path = path;
}


void VideoStreamService::setConferenceVideoQuality(int video_quality) {
    this->conference_video_quality = video_quality;
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
                // Increment the frame sequence ID
                Conference::instance().frameSeqID++;
                if (Conference::instance().frameSeqID > 60000) Conference::instance().frameSeqID = -1;

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


            } else { // Stop streaming

                // Destroy socket connection to the server
                socket.destroy();

            }

            stream_service.streaming = new_streaming_status;
        }

        //Streaming
        if (new_streaming_status) {


            // Send package request every 2 seconds
            if (Timer::calcTimePassed(stream_service.last_image_request_time) > 2000) {

                std::cout << Timer::calcTimePassed(stream_service.last_image_request_time) << std::endl;

                // Default
                int frame_width = 640;
                int frame_height = 480;
                int frame_quality = 50;

                switch (stream_service.conference_video_quality) {
                    case VIDEO_QUALITY_640x480_50: frame_width = 640; frame_height = 480; frame_quality = 50; break;
                    case VIDEO_QUALITY_640x480_80: frame_width = 640; frame_height = 480; frame_quality = 80; break;
                    case VIDEO_QUALITY_640x480_30: frame_width = 640; frame_height = 480; frame_quality = 30; break;
                    case VIDEO_QUALITY_320x240_80: frame_width = 320; frame_height = 240; frame_quality = 80; break;
                    case VIDEO_QUALITY_320x240_50: frame_width = 320; frame_height = 240; frame_quality = 50; break;
                    case VIDEO_QUALITY_320x240_30: frame_width = 320; frame_height = 240; frame_quality = 30; break;
                    default:  frame_width = 320; frame_height = 240; frame_quality = 30;
                }

                // Send stream request message
                socket.sendPacket(protocol_data.packageStreamRequestMessage(frame_width, frame_height, frame_quality));

                // Assign time
                stream_service.last_image_request_time = Timer::getCurrentTime();

            }

            std::vector<unsigned char> data = socket.getPacket();

            if (data.empty()) {
                continue;
            }

            Conference::instance().enqueueFrame(data);
            data = Conference::instance().dequeueFrame();

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

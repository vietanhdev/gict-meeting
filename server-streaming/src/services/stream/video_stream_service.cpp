#include "video_stream_service.h"
#include "opencv2/imgproc/imgproc.hpp"

VideoStreamService::VideoStreamService() {}

void VideoStreamService::videoUpService() {

    Conference &conference = Conference::instance();

    std::shared_ptr<ServerSocket> socket = VideoStreamService::instance().getVideoUpSocket();

    VideoFrameProtocolData protocol_data;
    while (true) {

        std::vector<unsigned char> data = socket->getPacket().data;

        if (data.empty()) {
            continue;
        }

        protocol_data.unpackHeader(data);

        // Wrong format
        if (protocol_data.getMessage() != Message::CLIENT_IMAGE_FRAME) {
            std::cerr << "Incoming packet in wrong format." << std::endl;
            continue;
        }

        // Authentication
        if (conference.checkAuth(protocol_data.getClientId(), protocol_data.getClientAuthKey())) {
            std::vector<unsigned char> payload_data;
            protocol_data.unpackData(data, payload_data);
            VideoFrame video_frame(payload_data);
            conference.setImage(protocol_data.getClientId(), video_frame.getImage());
            // video_frame.display();
        }

    }

}


void VideoStreamService::videoDownServiceListening() {

    Conference &conference = Conference::instance();
    std::shared_ptr<ServerSocket> socket = VideoStreamService::instance().getVideoDownSocket();

    VideoFrameProtocolData protocol_data;
    while (true) {

        std::vector<unsigned char> data = socket->getPacket().data;

        if (data.empty()) {
            continue;
        }

        protocol_data.unpackHeader(data); 

        // When client request image stream, update client address in participant
        if (protocol_data.getMessage() == Message::REQUEST_IMAGE_STREAM) {

            std::cout << "Client requested image stream. Client id = " << static_cast<int>(protocol_data.getClientId()) << std::endl;

            // Authentication
            if (conference.checkAuth(protocol_data.getClientId(), protocol_data.getClientAuthKey() )) {
                conference.connectClientImage(protocol_data.getClientId(), socket->getPacket().client_addr);
            }
        }

        
        
    }

}


void VideoStreamService::videoDownServiceSending() {
    Conference &conference = Conference::instance();
    std::shared_ptr<ServerSocket> socket = VideoStreamService::instance().getVideoDownSocket();
    VideoStreamService &stream_service = VideoStreamService::instance();
    VideoFrameProtocolData protocol_data;

    while (true) {

        // Check time to ensure keep sending in prefered fps
        if (Timer::calcTimePassed(stream_service.last_image_up_time) < 1000 / stream_service.frame_up_fps ) {
            stream_service.last_image_up_time = Timer::getCurrentTime();
            continue;
        }
      
        // Send to participant i
        for (int i = 0; i < conference.participants.size(); ++i) {

            // Send video to client if connected
            if (conference.participants[i].isConnectedImage()) {

                // List of images of all other participants
                std::vector<cv::Mat> images;

                // Send images from participant j
                for (int j = 0; j < conference.participants.size(); ++j) {

                    if (j != i || DEV_ECHO_IMAGE) {
                        // Push the image into the list of images
                        cv::Mat image = conference.participants[j].getImage();

                        if (image.size().width != 0) // Not sure why there is an empty image
                            images.push_back(image);
                    }
                }

                if (images.size() == 0) continue;

                // Return maximum of 4 images, so we will resize them in half
                cv::Size size(images[0].size().width / 2, images[0].size().height / 2);

                // Combine all images
                cv::Mat combinedImage;
                cv::Mat img0;
                cv::Mat img1;
                // Empty black image
                cv::Mat blankImg;

                switch(images.size()) {
                    case 1:
                        combinedImage = images[0];
                        break;
                    case 2:
                        cv::resize(images[0], img0, size);
                        cv::resize(images[1], img1, size);
                        hconcat(img0, img1, combinedImage);
                        break;
                    case 3:
                        cv::resize(images[0], img0, size);
                        cv::resize(images[1], img1, size);
                        hconcat(img0, img1, combinedImage);
                        cv::resize(images[2], img0, size);

                        // Create a blank image with the same size
                        blankImg = img0.clone();
                        blankImg.setTo(cv::Scalar(0, 0, 0)); // Set it to black
                        hconcat(img0, blankImg, img1);

                        vconcat(combinedImage, img1, combinedImage);
                        break;
                    default: // case >= 4
                        cv::resize(images[0], img0, size);
                        cv::resize(images[1], img1, size);
                        hconcat(img0, img1, combinedImage);
                        cv::resize(images[2], img0, size);
                        cv::resize(images[3], img1, size);
                        hconcat(img0, img1, img0);
                        vconcat(combinedImage, img0, combinedImage);
                        break;
                }

                // Increment frame seq ID
                conference.frameSeqID++;
                if (conference.frameSeqID > 60000) conference.frameSeqID = 0;

                // Return the combined image
                std::vector<unsigned char> message = protocol_data.packageData(conference.participants[i].getClientId(), combinedImage);
                socket->sendPackage(conference.participants[i].getClientImageAddress(), message);
            }

        }


    }
}

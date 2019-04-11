
#include "video_stream_service.h"

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
      
        for (int i = 0; i < conference.participants.size(); ++i) {

            // Send video to client if connected
            if (conference.participants[i].isConnectedImage()) {

                for (int j = 0; j < conference.participants.size(); ++j) {

                    if (j != i || DEV_ECHO_IMAGE) {
                        std::vector<unsigned char> message = protocol_data.packageData(conference.participants[j].getClientId(), conference.participants[j].getImage());
                        socket->sendPackage(conference.participants[i].getClientImageAddress(), message);
                    }
                    
                }

            }

        }


    }
}
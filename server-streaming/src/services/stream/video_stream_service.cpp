
#include "video_stream_service.h"

VideoStreamService::VideoStreamService() {}

void VideoStreamService::videoUpService() {

    Conference &conference = Conference::instance();

    std::shared_ptr<ReceiverSocket> socket = VideoStreamService::instance().getVideoUpSocket();

    VideoFrameProtocolData protocol_data;
    while (true) {
        protocol_data.unpackHeader(socket->getPacket().data);

        // Wrong format
        if (protocol_data.getMessage() != Message::CLIENT_FRAME) {
            std::cerr << "Incoming packet in wrong format." << std::endl;
            continue;
        }

        // Authentication
        if (conference.checkAuth(protocol_data.getClientId(), protocol_data.getClientAuthKey())) {
            std::vector<unsigned char> data;
            protocol_data.unpackData(socket->getPacket().data, data);
            VideoFrame video_frame(data);
            conference.setImage(protocol_data.getClientId(), video_frame.getImage());

            video_frame.display();
        }

    }

}


void VideoStreamService::videoDownService() {

    Conference &conference = Conference::instance();
    std::shared_ptr<ReceiverSocket> socket = VideoStreamService::instance().getVideoDownSocket();

    VideoFrameProtocolData protocol_data;
    while (true) {
        
        protocol_data.unpackHeader(socket->getPacket().data); 

        // When client request image stream, update client address in participant
        if (protocol_data.getMessage() == Message::REQUEST_IMAGE_STREAM) {
            // Authentication
            if (conference.checkAuth(protocol_data.getClientId(), protocol_data.getClientAuthKey() )) {
                conference.setClientAddress(protocol_data.getClientId(), socket->getPacket().client_addr);
            }
        }

    }

}
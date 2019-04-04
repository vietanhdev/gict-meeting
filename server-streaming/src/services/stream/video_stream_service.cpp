
#include "video_stream_service.h"

VideoStreamService::VideoStreamService() {}

void VideoStreamService::videoUpService() {

    Conference &conference = Conference::instance();

    std::shared_ptr<ServerSocket> socket = VideoStreamService::instance().getVideoUpSocket();

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


void VideoStreamService::videoDownServiceListening() {

    Conference &conference = Conference::instance();
    std::shared_ptr<ServerSocket> socket = VideoStreamService::instance().getVideoDownSocket();

    VideoFrameProtocolData protocol_data;
    while (true) {
        
        protocol_data.unpackHeader(socket->getPacket().data); 

        // When client request image stream, update client address in participant
        if (protocol_data.getMessage() == Message::REQUEST_IMAGE_STREAM) {
            // Authentication
            if (conference.checkAuth(protocol_data.getClientId(), protocol_data.getClientAuthKey() )) {
                conference.connectClient(protocol_data.getClientId(), socket->getPacket().client_addr);
            }
        }

    }

}


void VideoStreamService::videoDownServiceSending() {
    Conference &conference = Conference::instance();
    std::shared_ptr<ServerSocket> socket = VideoStreamService::instance().getVideoDownSocket();

    VideoFrameProtocolData protocol_data;
    while (true) {
        std::vector<Participant>& participants = conference.getParticipants();

        for (int i = 0; i > participants.size(); ++i) {

            // Send video to client if connected
            if (participants[i].isConnected()) {

                std::vector<unsigned char> message = protocol_data.packageData(participants[i].getClientId(), participants[i].getImage());
                socket->sendPackage(participants[i].getClientAddress(), message);

            }

        }

    }
}
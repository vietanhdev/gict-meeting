
#include "audio_stream_service.h"

AudioStreamService::AudioStreamService() {}

void AudioStreamService::audioUpService() {

    Conference &conference = Conference::instance();

    std::shared_ptr<ServerSocket> socket = AudioStreamService::instance().getAudioUpSocket();

    AudioFrameProtocolData protocol_data;
    while (true) {

        std::vector<unsigned char> data = socket->getPacket().data;

        if (data.empty()) {
            continue;
        }

        protocol_data.unpackHeader(data);

        // Wrong format
        if (protocol_data.getMessage() != Message::CLIENT_AUDIO_FRAME) {
            std::cerr << "Incoming packet in wrong format." << std::endl;
            continue;
        }

        // Authentication
        if (conference.checkAuth(protocol_data.getClientId(), protocol_data.getClientAuthKey())) {
            std::vector<unsigned char> payload_data;
            protocol_data.unpackData(data, payload_data);
            
            AudioStreamService::instance().sendAudio(protocol_data.getClientId(), protocol_data.unpackPayload(data));

            // std::cout << "Receive:" << payload_data.size() << std::endl;
            // std::cout << "Receive content:" << static_cast<int>(payload_data[30]) << std::endl;

        }

    }

}


void AudioStreamService::audioDownServiceListening() {

    Conference &conference = Conference::instance();
    std::shared_ptr<ServerSocket> socket = AudioStreamService::instance().getAudioDownSocket();

    AudioFrameProtocolData protocol_data;
    while (true) {

        std::vector<unsigned char> data = socket->getPacket().data;

        if (data.empty()) {
            continue;
        }

        protocol_data.unpackHeader(data); 

        // When client request image stream, update client address in participant
        if (protocol_data.getMessage() == Message::REQUEST_AUDIO_STREAM) {

            std::cout << "Client requested audio stream. Client id = " << static_cast<int>(protocol_data.getClientId()) << std::endl;

            // Authentication
            if (conference.checkAuth(protocol_data.getClientId(), protocol_data.getClientAuthKey() )) {
                conference.connectClientAudio(protocol_data.getClientId(), socket->getPacket().client_addr);
            }

        }
        
        
    }

}


void AudioStreamService::sendAudio(unsigned char source_client_id, const std::vector<unsigned char> &data) {
    Conference &conference = Conference::instance();
    std::shared_ptr<ServerSocket> socket = AudioStreamService::instance().getAudioDownSocket();

    AudioFrameProtocolData protocol_data;
    for (int i = 0; i < conference.participants.size(); ++i) {

        // Send audio to client if connected
        if (conference.participants[i].isConnectedAudio()) {

            std::vector<unsigned char> message = protocol_data.packageConferenceAudioFrame(conference.participants[i].getClientId(), data);
            socket->sendPackage(conference.participants[i].getClientAudioAddress(), message);

        }

    }
}

void AudioStreamService::audioDownServiceSending() {
    Conference &conference = Conference::instance();
    std::shared_ptr<ServerSocket> socket = AudioStreamService::instance().getAudioDownSocket();

    AudioFrameProtocolData protocol_data;
    while (true) {
      
        // for (int i = 0; i < conference.participants.size(); ++i) {

        //     // Send audio to client if connected
        //     if (conference.participants[i].isConnected()) {

        //         std::vector<unsigned char> message = protocol_data.packageData(conference.participants[i].getClientId(), conference.participants[i].getImage());
        //         socket->sendPackage(conference.participants[i].getClientAddress(), message);

        //     }

        // }


    }
}
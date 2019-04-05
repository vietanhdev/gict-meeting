#include "audio_frame_protocol.h"

#include <vector>


std::vector<unsigned char> AudioFrameProtocolData::packageData() const {
    std::vector<unsigned char> message;
    return message;
}


std::vector<unsigned char> AudioFrameProtocolData::packageConferenceAudioFrame(unsigned char client_id, const std::vector<unsigned char> &data) const {

    std::vector<unsigned char> message = data;

    // 14 dummy bytes
    // May be used in the future for frame data
    for (int i = 0; i < 14; ++i) {
        message.insert(message.begin(), 0);
    }

    // Insert client id
    message.insert(message.begin(), client_id);

    // Insert message type
    message.insert(message.begin(), Message::CONFERENCE_AUDIO_FRAME);

    return message;

}

std::vector<unsigned char> AudioFrameProtocolData::unpackPayload( const std::vector<unsigned char>& raw_bytes) {

    if (raw_bytes.size() <= 16) {
        return std::vector<unsigned char>();
    }

    // Extract frame data
    std::vector<unsigned char>::const_iterator first = raw_bytes.begin() + 16;
    std::vector<unsigned char>::const_iterator last = raw_bytes.end();
    std::vector<unsigned char> audio_frame(first, last);

    return audio_frame;
}



// Unpack the header of message (the first 16 bytes)
// Return true if unpack successfully
// Otherwise return false
bool AudioFrameProtocolData::unpackHeader( const std::vector<unsigned char>& raw_bytes) {
    // Corrupted package
    if (raw_bytes.size() < 16) {
        std::cerr << "Too short packet. " << raw_bytes.size() << std::endl;
        return false;
    }

    message = raw_bytes[0];
    client_id = raw_bytes[1];
    client_auth_key = (raw_bytes[2] << 24) + (raw_bytes[3] << 16) + (raw_bytes[4] << 8) + raw_bytes[5];

    // Extract header
    std::vector<unsigned char>::const_iterator first = raw_bytes.begin() + 6;
    std::vector<unsigned char>::const_iterator last = raw_bytes.begin() + 16;
    
    header_data = std::vector<unsigned char>(first, last);

    return true;
}


// Return true if unpack successfully
// Otherwise return false
void AudioFrameProtocolData::unpackData( const std::vector<unsigned char>& raw_bytes, std::vector<unsigned char>& data ) {
    if (raw_bytes.size() < 16) {
        data.clear();
        return;
    }
    // Extract frame data
    std::vector<unsigned char>::const_iterator first = raw_bytes.begin() + 16;
    std::vector<unsigned char>::const_iterator last = raw_bytes.end();
    data = std::vector<unsigned char>(first, last);
}


unsigned char AudioFrameProtocolData::getClientId() const {
    return client_id;
}

int AudioFrameProtocolData::getClientAuthKey() const {
    return client_auth_key;
}


// Return message of packet
unsigned char AudioFrameProtocolData::getMessage() const {
    return message;
}

const std::vector<unsigned char>& AudioFrameProtocolData::getHeaderData() const {
    return header_data;
}

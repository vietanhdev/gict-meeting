#include "audio_frame_protocol.h"

#include <vector>

std::vector<unsigned char> AudioFrameProtocolData::packageClientFrame(const std::vector<unsigned char> & data) const {

    std::vector<unsigned char> message = data;

    // 10 dummy bytes
    // May be used in the future for frame data
    for (int i = 0; i < 10; ++i) {
        message.insert(message.begin(), 0);
    }

    // 4 bytes auth key
    int n = Conference::instance().getClientAuthKey();
    message.insert(message.begin(), n & 0xFF);
    message.insert(message.begin(), (n >> 8) & 0xFF); 
    message.insert(message.begin(), (n >> 16) & 0xFF); 
    message.insert(message.begin(), (n >> 24) & 0xFF);

    // Insert client id
    message.insert(message.begin(), Conference::instance().getClientId());

    // Insert message type
    message.insert(message.begin(), Message::CLIENT_AUDIO_FRAME);

    return message;
}



std::vector<unsigned char> AudioFrameProtocolData::packageStreamRequestMessage() const {

    std::vector<unsigned char> message;

    // 10 dummy bytes
    // May be used in the future for frame data
    for (int i = 0; i < 10; ++i) {
        message.insert(message.begin(), 0);
    }

    // 4 bytes auth key
    int n = Conference::instance().getClientAuthKey();
    message.insert(message.begin(), n & 0xFF);
    message.insert(message.begin(), (n >> 8) & 0xFF); 
    message.insert(message.begin(), (n >> 16) & 0xFF); 
    message.insert(message.begin(), (n >> 24) & 0xFF);

    // Insert client id
    message.insert(message.begin(), Conference::instance().getClientId());

    // Insert message type
    message.insert(message.begin(), Message::REQUEST_AUDIO_STREAM);

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

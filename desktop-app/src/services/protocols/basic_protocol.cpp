#include "protocols/basic_protocol.h"

#include <vector>

std::vector<unsigned char> BasicProtocolData::packageData() const {
    std::vector<unsigned char> message = video_frame_.getJPEG();

    // 4 dummy byte.
    // May be used in the future for frame data
    message.insert(message.begin(), 0);
    message.insert(message.begin(), 0); 
    message.insert(message.begin(), 0); 
    message.insert(message.begin(), 0);

    // 4 byte auth key
    int n = Conference::instance().getClientAuthKey();
    message.insert(message.begin(), n & 0xFF);
    message.insert(message.begin(), (n >> 8) & 0xFF); 
    message.insert(message.begin(), (n >> 16) & 0xFF); 
    message.insert(message.begin(), (n >> 24) & 0xFF);

    // Insert client id
    message.insert(message.begin(), Conference::instance().getClientId());

    // Insert message type
    message.insert(message.begin(), MessageType::CLIENT_FRAME);

}


// Return true if unpack successfully
// Otherwise return false
bool BasicProtocolData::unpackData( const std::vector<unsigned char>& raw_bytes) {

    // Corrupted package
    if (raw_bytes.size() <= 6) {
        return false;
    }

    // Wrong format
    if (raw_bytes[0] != MessageType::CLIENT_FRAME) {
        return false;
    }

    client_id = raw_bytes[1];
    client_auth_key = (raw_bytes[2] << 24) + (raw_bytes[3] << 16) + (raw_bytes[4] << 8) + raw_bytes[5];

    // Extract frame data
    std::vector<unsigned char>::const_iterator first = raw_bytes.begin() + 6;
    std::vector<unsigned char>::const_iterator last = raw_bytes.end();
    std::vector<unsigned char> video_frame_bytes(first, last);

    video_frame_ = VideoFrame(video_frame_bytes);
}


unsigned char BasicProtocolData::getClientId() const {
    return client_id;
}

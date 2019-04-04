#include "video_frame_protocol.h"

#include <vector>

std::vector<unsigned char> VideoFrameProtocolData::packageData() const {
    std::vector<unsigned char> message = video_frame_.getJPEG();

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
    message.insert(message.begin(), Message::CLIENT_FRAME);

    return message;

}


// Return true if unpack successfully
// Otherwise return false
bool VideoFrameProtocolData::unpackData( const std::vector<unsigned char>& raw_bytes) {

    
}


unsigned char VideoFrameProtocolData::getClientId() const {
    return client_id;
}

int VideoFrameProtocolData::getClientAuthKey() const {
    return client_auth_key;
}

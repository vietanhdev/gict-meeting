#include "video_frame_protocol.h"

#include <vector>

std::vector<unsigned char> VideoFrameProtocolData::packageData() const {
    std::vector<unsigned char> message = video_frame_.getJPEG();
    // Insert message type
    message.insert(message.begin(), MessageType::CLIENT_FRAME);
}


// Return true if unpack successfully
// Otherwise return false
bool VideoFrameProtocolData::unpackData( const std::vector<unsigned char>& raw_bytes) {

    // Corrupted package
    if (raw_bytes.size() <= 16) {
        std::cerr << "Too short packet. " << raw_bytes.size() << std::endl;
        return false;
    }

    // Wrong format
    if (raw_bytes[0] != MessageType::CLIENT_FRAME) {
        std::cerr << "Incoming packet in wrong format." << std::endl;
        return false;
    }

    client_id = raw_bytes[1];
    client_auth_key = (raw_bytes[2] << 24) + (raw_bytes[3] << 16) + (raw_bytes[4] << 8) + raw_bytes[5];

    // Extract frame data
    std::vector<unsigned char>::const_iterator first = raw_bytes.begin() + 16;
    std::vector<unsigned char>::const_iterator last = raw_bytes.end();
    std::vector<unsigned char> video_frame_bytes(first, last);

    video_frame_ = VideoFrame(video_frame_bytes);
}


unsigned char VideoFrameProtocolData::getClientId() const {
    return client_id;
}

int VideoFrameProtocolData::getClientAuthKey() const {
    return client_auth_key;
}

#include "video_frame_protocol.h"

#include <vector>

std::vector<unsigned char> VideoFrameProtocolData::packageData() const {
    std::vector<unsigned char> message;
    return message;
}


// Unpack the header of message (the first 16 bytes)
// Return true if unpack successfully
// Otherwise return false
bool VideoFrameProtocolData::unpackHeader( const std::vector<unsigned char>& raw_bytes) {
    // Corrupted package
    if (raw_bytes.size() <= 16) {
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
void VideoFrameProtocolData::unpackData( const std::vector<unsigned char>& raw_bytes, std::vector<unsigned char>& data ) {
    // Extract frame data
    std::vector<unsigned char>::const_iterator first = raw_bytes.begin() + 16;
    std::vector<unsigned char>::const_iterator last = raw_bytes.end();
    data = std::vector<unsigned char>(first, last);
}


unsigned char VideoFrameProtocolData::getClientId() const {
    return client_id;
}

int VideoFrameProtocolData::getClientAuthKey() const {
    return client_auth_key;
}


// Return message of packet
unsigned char VideoFrameProtocolData::getMessage() const {
    return message;
}

const std::vector<unsigned char>& VideoFrameProtocolData::getHeaderData() const {
    return header_data;
}

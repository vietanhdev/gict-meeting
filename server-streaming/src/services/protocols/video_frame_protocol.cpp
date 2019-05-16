#include "video_frame_protocol.h"

#include <vector>


std::vector<unsigned char> VideoFrameProtocolData::packageData() const {
    std::vector<unsigned char> message;
    return message;
}


std::vector<unsigned char> VideoFrameProtocolData::packageData(unsigned char client_id, const cv::Mat & img) const {

    VideoFrame video_frame(img);

    std::vector<unsigned char> message = video_frame.getJPEG();

    // 14 dummy bytes
    // May be used in the future for frame data
    for (int i = 0; i < 8; ++i) {
        message.insert(message.begin(), 0);
    }

    // 2 bytes frame sequence ID
    message.insert(message.begin(), Conference::instance().frameSeqID & 0xFF);
    message.insert(message.begin(), (Conference::instance().frameSeqID >> 8) & 0xFF); 

    // 4 dummy bytes for auth
    for (int i = 0; i < 4; i++) {
        message.insert(message.begin(), 0);
    }

    // Insert client id
    message.insert(message.begin(), client_id);

    // Insert message type
    message.insert(message.begin(), Message::CONFERENCE_IMAGE_FRAME);

    return message;

}


// Unpack the header of message (the first 16 bytes)
// Return true if unpack successfully
// Otherwise return false
bool VideoFrameProtocolData::unpackHeader( const std::vector<unsigned char>& raw_bytes) {
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
void VideoFrameProtocolData::unpackData( const std::vector<unsigned char>& raw_bytes, std::vector<unsigned char>& data ) {
    if (raw_bytes.size() < 16) {
        data.clear();
        return;
    }
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

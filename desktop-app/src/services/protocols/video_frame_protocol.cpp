#include "video_frame_protocol.h"

#include <vector>

std::vector<unsigned char> VideoFrameProtocolData::packageClientFrame(const cv::Mat & img) const {

    VideoFrame video_frame(img);

    std::vector<unsigned char> message = video_frame.getJPEG();

    // 10 dummy bytes
    // May be used in the future for frame data
    for (int i = 0; i < 8; ++i) {
        message.insert(message.begin(), 0);
    }
    
    // 2 bytes frame sequence ID
    message.insert(message.begin(), Conference::instance().frameSeqID & 0xFF);
    message.insert(message.begin(), (Conference::instance().frameSeqID >> 8) & 0xFF); 

    // 4 bytes auth key
    int n = Conference::instance().getClientAuthKey();
    message.insert(message.begin(), n & 0xFF);
    message.insert(message.begin(), (n >> 8) & 0xFF); 
    message.insert(message.begin(), (n >> 16) & 0xFF); 
    message.insert(message.begin(), (n >> 24) & 0xFF);

    // Insert client id
    message.insert(message.begin(), Conference::instance().getClientId());

    // Insert message type
    message.insert(message.begin(), Message::CLIENT_IMAGE_FRAME);

    return message;
}



std::vector<unsigned char> VideoFrameProtocolData::packageStreamRequestMessage(int frame_width, int frame_height, int frame_quality) const {

    std::vector<unsigned char> message;

    // 5 dummy bytes
    // May be used in the future for frame data
    for (int i = 0; i < 10; ++i) {
        message.insert(message.begin(), 0);
    }

    // Frame Quality
    message.insert(message.begin(), frame_quality & 0xFF);

    // Frame Height
    message.insert(message.begin(), frame_height & 0xFF);
    message.insert(message.begin(), (frame_height >> 8) & 0xFF); 

    // Frame Width
    message.insert(message.begin(), frame_width & 0xFF);
    message.insert(message.begin(), (frame_width >> 8) & 0xFF); 

    // 4 bytes auth key
    int n = Conference::instance().getClientAuthKey();
    message.insert(message.begin(), n & 0xFF);
    message.insert(message.begin(), (n >> 8) & 0xFF); 
    message.insert(message.begin(), (n >> 16) & 0xFF); 
    message.insert(message.begin(), (n >> 24) & 0xFF);

    // Insert client id
    message.insert(message.begin(), Conference::instance().getClientId());

    // Insert message type
    message.insert(message.begin(), Message::REQUEST_IMAGE_STREAM);

    return message;
}


cv::Mat VideoFrameProtocolData::unpackConferenceFrame( const std::vector<unsigned char>& raw_bytes) {
    cv::Mat frame;

    if (raw_bytes.size() < 16) {
        std::cerr << "Too short packet. " << raw_bytes.size() << std::endl;
        return frame;
    }

    // Check message type
    unsigned char message = raw_bytes[0];
    if (message != Message::CONFERENCE_IMAGE_FRAME) {
        std::cout << "Wrong message from server" << std::endl;
        return frame;
    }

    // Extract frame data
    std::vector<unsigned char>::const_iterator first = raw_bytes.begin() + 16;
    std::vector<unsigned char>::const_iterator last = raw_bytes.end();
    std::vector<unsigned char> data(first, last);

    VideoFrame video_frame(data);
    frame = video_frame.getImage();

    return frame;
}

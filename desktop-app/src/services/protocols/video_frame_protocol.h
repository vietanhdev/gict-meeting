// The basic protocol transmits raw, uncompressed video frames only. This is
// the most basic video protocol.

#ifndef VIDEO_FRAME_PROTOCOL_H
#define VIDEO_FRAME_PROTOCOL_H

#include <vector>
#include <iostream>
#include "video_frame.h"
#include "conference.h"
#include "message_type.h"

class VideoFrameProtocolData {
   public: 
    std::vector<unsigned char> packageClientFrame(const cv::Mat & img, int frameSeqID) const;
    std::vector<unsigned char> packageStreamRequestMessage() const;

    // Return true if unpack successfully
    // Otherwise return false
    bool unpackData(const std::vector<unsigned char>& raw_bytes);
    cv::Mat unpackConferenceFrame( const std::vector<unsigned char>& raw_bytes);

};

#endif  // VIDEO_FRAME_PROTOCOL_H

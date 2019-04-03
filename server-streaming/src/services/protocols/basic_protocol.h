// The basic protocol transmits raw, uncompressed video frames only. This is
// the most basic video protocol.

#ifndef PROTOCOLS_BASIC_PROTOCOL_H
#define PROTOCOLS_BASIC_PROTOCOL_H

#include <vector>

#include "protocols/protocol.h"
#include "video/video_frame.h"

class BasicProtocolData : public ProtocolData {
   public:
    std::vector<unsigned char> packageData() const override;

    void unpackData(const std::vector<unsigned char>& raw_bytes) override;

    // Sets the next video frame.
    void setImage(const VideoFrame& image) { video_frame_ = image; }

    // Returns the video frame image.
    VideoFrame getImage() const { return video_frame_; }

   private:
    // The video frame received from the packet is stored here.
    VideoFrame video_frame_;
};

#endif  // PROTOCOLS_BASIC_PROTOCOL_H

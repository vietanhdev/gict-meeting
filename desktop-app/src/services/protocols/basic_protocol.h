// The basic protocol transmits raw, uncompressed video frames only. This is
// the most basic video protocol.

#ifndef PROTOCOLS_BASIC_PROTOCOL_H
#define PROTOCOLS_BASIC_PROTOCOL_H

#include <vector>

#include "protocols/protocol.h"
#include "video/video_frame.h"

namespace udp_streaming_video {

class BasicProtocolData : public ProtocolData {
   public:
    std::vector<unsigned char> PackageData() const override;

    void UnpackData(const std::vector<unsigned char>& raw_bytes) override;

    // Sets the next video frame.
    void SetImage(const VideoFrame& image) { video_frame_ = image; }

    // Returns the video frame image.
    VideoFrame GetImage() const { return video_frame_; }

   private:
    // The video frame received from the packet is stored here.
    VideoFrame video_frame_;
};

}  // namespace udp_streaming_video

#endif  // PROTOCOLS_BASIC_PROTOCOL_H

#include "protocols/basic_protocol.h"

#include <vector>

std::vector<unsigned char> BasicProtocolData::packageData() const {
    return video_frame_.getJPEG();
}

void BasicProtocolData::unpackData(
    const std::vector<unsigned char>& raw_bytes) {
    video_frame_ = VideoFrame(raw_bytes);
}

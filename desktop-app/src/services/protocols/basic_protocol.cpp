#include "protocols/basic_protocol.h"

#include <vector>

std::vector<unsigned char> BasicProtocolData::PackageData() const {
    return video_frame_.GetJPEG();
}

void BasicProtocolData::UnpackData(
    const std::vector<unsigned char>& raw_bytes) {
    video_frame_ = VideoFrame(raw_bytes);
}

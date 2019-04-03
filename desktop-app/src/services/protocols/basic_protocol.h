// The basic protocol transmits raw, uncompressed video frames only. This is
// the most basic video protocol.

#ifndef PROTOCOLS_BASIC_PROTOCOL_H
#define PROTOCOLS_BASIC_PROTOCOL_H

#include <vector>
#include <iostream>
#include "protocols/protocol.h"
#include "video/video_frame.h"
#include "conference.h"
#include "message_type.h"

class BasicProtocolData : public ProtocolData {
   public:
    std::vector<unsigned char> packageData() const override;

    // Return true if unpack successfully
    // Otherwise return false
    bool unpackData(const std::vector<unsigned char>& raw_bytes) override;

    // Sets the next video frame.
    void setImage(const VideoFrame& image) { video_frame_ = image; }

    // Returns the video frame image.
    VideoFrame getImage() const { return video_frame_; }

   // Return client id of video frame (Who sent this frame?)
    unsigned char getClientId() const;

    int getClientAuthKey() const;

   private:
    unsigned char client_id;
    int client_auth_key;
    // The video frame received from the packet is stored here.
    VideoFrame video_frame_;


};

#endif  // PROTOCOLS_BASIC_PROTOCOL_H

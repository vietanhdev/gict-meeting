// The basic protocol transmits raw, uncompressed video frames only. This is
// the most basic video protocol.

#ifndef VIDEO_FRAME_PROTOCOL_H
#define VIDEO_FRAME_PROTOCOL_H

#include <vector>

#include "protocol.h"
#include "video_frame.h"
#include "conference.h"
#include "message_type.h"

class VideoFrameProtocolData : public ProtocolData {
   public:
    std::vector<unsigned char> packageData() const override;
    std::vector<unsigned char> packageData(unsigned char client_id, const cv::Mat & img, int width, int height, int quality) const;

    // Return true if unpack successfully
    // Otherwise return false
    bool unpackHeader(const std::vector<unsigned char>& raw_bytes) override;

    void unpackData( const std::vector<unsigned char>& raw_bytes, std::vector<unsigned char>& data );

    // Return client id of packet (Who sent this packet?)
    unsigned char getClientId() const;

    int getClientAuthKey() const;

    // Return message of packet
    unsigned char getMessage() const;

    // Get down stream quality
    static std::vector<int> getDownStreamQuality(const std::vector<unsigned char>& raw_bytes);

    const std::vector<unsigned char>& getHeaderData() const;

   private:
    unsigned char client_id;
    int client_auth_key;
    unsigned char message;
    std::vector<unsigned char> header_data;


};

#endif  // VIDEO_FRAME_PROTOCOL_H

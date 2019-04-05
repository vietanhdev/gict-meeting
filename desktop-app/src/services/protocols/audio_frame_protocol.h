#ifndef AUDIO_FRAME_PROTOCOL_H
#define AUDIO_FRAME_PROTOCOL_H

#include <vector>
#include <iostream>
#include "conference.h"
#include "message_type.h"

class AudioFrameProtocolData {
   public: 
    std::vector<unsigned char> packageClientFrame(const std::vector<unsigned char> & data) const;
    std::vector<unsigned char> packageStreamRequestMessage() const;

    std::vector<unsigned char> unpackPayload( const std::vector<unsigned char>& raw_bytes);

};

#endif  // AUDIO_FRAME_PROTOCOL_H

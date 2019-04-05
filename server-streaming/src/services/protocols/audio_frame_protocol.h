// The basic protocol transmits raw, uncompressed audio frames only. This is
// the most basic audio protocol.

#ifndef AUDIO_FRAME_PROTOCOL_H
#define AUDIO_FRAME_PROTOCOL_H

#include <vector>

#include "protocol.h"
#include "conference.h"
#include "message_type.h"

class AudioFrameProtocolData : public ProtocolData {
   public:
    std::vector<unsigned char> packageData() const override;
    std::vector<unsigned char> packageConferenceAudioFrame(unsigned char client_id, const std::vector<unsigned char> &data)  const;
    std::vector<unsigned char> unpackPayload( const std::vector<unsigned char>& raw_bytes);

    // Return true if unpack successfully
    // Otherwise return false
    bool unpackHeader(const std::vector<unsigned char>& raw_bytes) override;

    void unpackData( const std::vector<unsigned char>& raw_bytes, std::vector<unsigned char>& data );

    // Return client id of packet (Who sent this packet?)
    unsigned char getClientId() const;

    int getClientAuthKey() const;

    // Return message of packet
    unsigned char getMessage() const;

    const std::vector<unsigned char>& getHeaderData() const;

   private:
    unsigned char client_id;
    int client_auth_key;
    unsigned char message;
    std::vector<unsigned char> header_data;


};

#endif  // AUDIO_FRAME_PROTOCOL_H

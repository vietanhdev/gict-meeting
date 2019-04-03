// A ProtocolData object interprets incoming packet data, which is provided in
// raw bytes. The bytes are turned the appropriate variables, which are stored
// in the binary packet data according by the specific protocol.

#ifndef PROTOCOLS_PROTOCOL_H
#define PROTOCOLS_PROTOCOL_H

#include <vector>

class ProtocolData {
   public:
    // Puts all of the relevant variables into a raw byte buffer which is
    // returned, ready to be sent as a packet over the network.
    virtual std::vector<unsigned char> PackageData() const = 0;

    // Unpacks a received data buffer into the appropriate variables, in
    // accordance to the specific protocol being implemented.
    virtual void UnpackData(const std::vector<unsigned char>& raw_bytes) = 0;
};

#endif  // PROTOCOLS_PROTOCOL_H

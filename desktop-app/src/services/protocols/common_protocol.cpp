#include "common_protocol.h"

bool CommonProtocolData::isAccessDenied(const std::vector<unsigned char>& raw_bytes) {
    if (raw_bytes.empty()) return false;
    return raw_bytes[0] == Message::ACCESS_DENIED;
}
#include "common_protocol.h"

std::vector<unsigned char> CommonProtocolData::getAccessDeniedMessage() {

    std::vector<unsigned char> message;

    // Insert message type
    message.insert(message.begin(), Message::ACCESS_DENIED);
    return message;

}
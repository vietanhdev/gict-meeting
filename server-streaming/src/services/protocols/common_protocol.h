#ifndef COMMON_PROTOCOL_H
#define COMMON_PROTOCOL_H

#include <vector>


#include "protocol.h"
#include "conference.h"
#include "message_type.h"

class CommonProtocolData : public ProtocolData {
   public:
    static std::vector<unsigned char> getAccessDeniedMessage();
};

#endif
#ifndef COMMON_PROTOCOL_H
#define COMMON_PROTOCOL_H

#include <vector>

#include "conference.h"
#include "message_type.h"

class CommonProtocolData  {
   public:
    static bool isAccessDenied(const std::vector<unsigned char>& raw_bytes);
};

#endif
// Copyright © 2017 Matt Robinson

#include <cstring>

#include "IdentifyMessage.h"

struct identification
{
    uint32_t : 24;
    uint8_t f2 : 8;
    uint8_t f3 : 8;
    uint8_t f4 : 8;
    uint64_t : 40;
    uint64_t f1 : 40;
} __attribute__((__packed__));

IdentifyMessage::IdentifyMessage(const std::vector<uint8_t> &raw)
{
    if(raw.size() < sizeof(identification))
    {
        throw std::runtime_error("Not enough data!");
    }

    struct identification unpack;
    memcpy(&unpack, &raw[0], sizeof(unpack));

    this->values["Software version"] = SpecialDataConv6(unpack.f2);
    this->values["Parameter version"] = SpecialDataConv6(unpack.f3);
    this->values["Parameter type"] = FormatNumber(unpack.f4, 1);
    this->values["Serial number"] = SpecialDataConv5(unpack.f1);
}

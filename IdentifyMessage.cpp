// Copyright © 2017 Matt Robinson

#include <cstring>

#include "IdentifyMessage.h"

struct identification
{
    uint32_t : 24;
    uint8_t f2010 : 8;
    uint8_t f2012 : 8;
    uint8_t f2013 : 8;
    uint64_t : 40;
    uint64_t f2015 : 40;
} __attribute__((__packed__));

IdentifyMessage::IdentifyMessage(const std::vector<uint8_t> &raw)
{
    if(raw.size() < sizeof(identification))
    {
        throw std::runtime_error("Not enough data!");
    }

    struct identification unpack;
    memcpy(&unpack, &raw[0], sizeof(unpack));

    this->values["Software version"] = SpecialDataConv6(unpack.f2010);
    this->values["Parameter version"] = SpecialDataConv6(unpack.f2012);
    this->values["Parameter type"] = std::to_string(unpack.f2013);
    this->values["Serial number"] = SpecialDataConv5(unpack.f2015);
}

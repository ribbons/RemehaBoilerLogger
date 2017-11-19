// Copyright © 2017 Matt Robinson

#include <cstring>
#include <iomanip>
#include <sstream>

#include <arpa/inet.h>
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

std::string IdentifyMessage::SpecialDataConv5(uint64_t data)
{
    uint16_t num1 =  data & 0x000000FFFF;
    char     lett = (data & 0x0000FF0000) >> 16;
    uint16_t num2 = (data & 0xFFFF000000) >> 24;

    std::stringstream stream;
    stream << std::hex << std::setfill('0') << std::setw(4) << ntohs(num1)
           << lett << std::setw(4) << ntohs(num2);

    return stream.str();
}

std::string IdentifyMessage::SpecialDataConv6(uint8_t data)
{
    uint hi = (data & 0xF0) >> 4;
    uint lo = data & 0x0F;

    std::stringstream stream;
    stream << std::hex << hi << '.' << lo;

    return stream.str();
}

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

std::map<std::string, std::string> IdentifyMessage::getValues()
{
    return this->values;
}

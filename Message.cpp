// Copyright © 2017 Matt Robinson

#include <iomanip>
#include <sstream>

#include <arpa/inet.h>
#include "IdentifyMessage.h"

Message Message::FromFrame(Frame frame)
{
    switch(frame.getFunction())
    {
        case FrameFunction::Identify:
            return IdentifyMessage(frame.getData());
    }

    throw std::runtime_error("Unknown frame type");
}

std::string Message::SpecialDataConv5(uint64_t data)
{
    uint16_t num1 =  data & 0x000000FFFF;
    char     lett = (data & 0x0000FF0000) >> 16;
    uint16_t num2 = (data & 0xFFFF000000) >> 24;

    std::stringstream stream;
    stream << std::hex << std::setfill('0') << std::setw(4) << ntohs(num1)
           << lett << std::setw(4) << ntohs(num2);

    return stream.str();
}

std::string Message::SpecialDataConv6(uint8_t data)
{
    uint hi = (data & 0xF0) >> 4;
    uint lo = data & 0x0F;

    std::stringstream stream;
    stream << std::hex << hi << '.' << lo;

    return stream.str();
}

std::map<std::string, std::string> Message::getValues()
{
    return this->values;
}

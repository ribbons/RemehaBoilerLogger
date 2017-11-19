// Copyright © 2017 Matt Robinson

#include <cstdint>
#include <map>
#include <vector>

#include "Frame.h"

class Message
{
protected:
    std::map<std::string, std::string> values;

    std::string SpecialDataConv5(uint64_t data);
    std::string SpecialDataConv6(uint8_t data);

public:
    std::map<std::string, std::string> getValues();
    static Message FromFrame(Frame frame);
};

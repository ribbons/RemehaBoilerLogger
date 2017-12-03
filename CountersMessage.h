// Copyright © 2017 Matt Robinson

#include "Message.h"

class CountersMessage : public Message
{
public:
    explicit CountersMessage(const std::vector<uint8_t> &raw);
};

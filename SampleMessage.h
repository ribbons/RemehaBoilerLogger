// Copyright © 2017 Matt Robinson

#include "Message.h"

class SampleMessage : public Message
{
public:
    explicit SampleMessage(const std::vector<uint8_t> &raw);
};

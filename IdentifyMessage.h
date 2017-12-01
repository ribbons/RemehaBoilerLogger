// Copyright © 2017 Matt Robinson

#include "Message.h"

class IdentifyMessage : public Message
{
public:
    explicit IdentifyMessage(const std::vector<uint8_t> &raw);
};

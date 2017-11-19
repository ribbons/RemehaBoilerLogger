// Copyright © 2017 Matt Robinson

#include "Message.h"

class IdentifyMessage : public Message
{
    friend Message Message::FromFrame(Frame frame);

protected:
    explicit IdentifyMessage(const std::vector<uint8_t> &raw);
};

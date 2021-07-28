/*
 * Copyright © 2017 Matt Robinson
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "Message.h"

class IdentifyMessage : public Message
{
public:
    explicit IdentifyMessage(const std::vector<uint8_t> &raw);
};

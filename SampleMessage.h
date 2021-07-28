/*
 * Copyright © 2017 Matt Robinson
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "Message.h"

class SampleMessage : public Message
{
public:
    explicit SampleMessage(const std::vector<uint8_t> &raw);
};

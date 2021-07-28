/*
 * Copyright © 2017 Matt Robinson
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "Message.h"

class CountersMessage : public Message
{
public:
    explicit CountersMessage(const std::vector<uint8_t> &raw);
};

/*
 * Copyright © 2017-2021 Matt Robinson
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <cstring>
#include <stdexcept>

#include "IdentifyMessage.h"

struct identification
{
    uint32_t : 24;
    uint8_t f4 : 8;
    uint8_t f5 : 8;
    uint8_t f6 : 8;
    uint64_t : 40;
    uint64_t f3 : 40;
    uint8_t f1 : 8;
    uint8_t f2 : 8;
} __attribute__((__packed__));

IdentifyMessage::IdentifyMessage(const std::vector<uint8_t> &raw)
{
    if(raw.size() < sizeof(identification))
    {
        throw std::runtime_error("Not enough data!");
    }

    struct identification unpack;
    memcpy(&unpack, &raw[0], sizeof(unpack));

    this->values["Software version"] = SpecialDataConv6(unpack.f4);
    this->values["Parameter version"] = SpecialDataConv6(unpack.f5);
    this->values["Parameter type"] = FormatNumber(unpack.f6, 1);
    this->values["Serial number"] = SpecialDataConv5(unpack.f3);
    this->values["dF-code"] = FormatNumber(unpack.f1, 1);
    this->values["dU-code"] = FormatNumber(unpack.f2, 1);
}

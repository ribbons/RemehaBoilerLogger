/*
 * This file is part of Remeha Boiler Logger.
 * Copyright © 2017-2021 by the authors - see the AUTHORS file for details.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
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

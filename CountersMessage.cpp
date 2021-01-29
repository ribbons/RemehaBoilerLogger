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
#include <arpa/inet.h>
#include "CountersMessage.h"

struct counter
{
    uint64_t : 48;
    uint16_t f1 : 16;
    uint16_t f2 : 16;
    uint16_t f4 : 16;
    uint16_t f3 : 16;
    uint16_t f5 : 16;
    uint16_t f6 : 16;
    uint16_t f7 : 16;
    uint16_t f8 : 16;
    uint16_t f9 : 16;
    uint16_t f10 : 16;
    uint16_t f11 : 16;
} __attribute__((__packed__));

CountersMessage::CountersMessage(const std::vector<uint8_t> &raw)
{
    if(raw.size() < sizeof(counter))
    {
        throw std::runtime_error("Not enough data!");
    }

    struct counter unpack;
    memcpy(&unpack, &raw[0], sizeof(unpack));

    this->values["Hours run pump CH+DHW"] = FormatNumber(ntohs(unpack.f1), 8);
    this->values["Hours run 3-way valve DHW"] = FormatNumber(ntohs(unpack.f2), 8);
    this->values["Hours run CH+DHW"] = FormatNumber(ntohs(unpack.f4), 8);
    this->values["Hours run DHW"] = FormatNumber(ntohs(unpack.f3), 8);
    this->values["Power supply available hrs"] = FormatNumber(ntohs(unpack.f5), 8);
    this->values["Pump starts CH+DHW"] = FormatNumber(ntohs(unpack.f6), 8);
    this->values["Number of 3way valve cycles"] = FormatNumber(ntohs(unpack.f7), 8);
    this->values["Burner starts DHW"] = FormatNumber(ntohs(unpack.f8), 8);
    this->values["Total Burner starts CH+DHW"] = FormatNumber(ntohs(unpack.f9), 8);
    this->values["Failed burner starts"] = FormatNumber(ntohs(unpack.f10), 8);
    this->values["Number of flame loss"] = FormatNumber(ntohs(unpack.f11), 8);
}

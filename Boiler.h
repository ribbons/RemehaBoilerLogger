/*
 * This file is part of Remeha Boiler Logger.
 * Copyright © 2017 by the authors - see the AUTHORS file for details.
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

#include "Serial.h"
#include "Frame.h"
#include "CountersMessage.h"
#include "IdentifyMessage.h"
#include "SampleMessage.h"

class Boiler
{
private:
    Serial port;
    std::vector<uint8_t> FetchData(FrameFunction function);

public:
    explicit Boiler(const std::string& path) : port(path) {};

    std::vector<uint8_t> ReadEepromBlock(uint8_t blockNum);
    std::vector<uint8_t> ReadEepromBlocks(uint8_t blockNum, uint8_t count);

    IdentifyMessage ReadIdentifyData();
    SampleMessage ReadSampleData();
    CountersMessage ReadCountersData();
};

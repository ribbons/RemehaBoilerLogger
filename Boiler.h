/*
 * Copyright © 2017 Matt Robinson
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
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

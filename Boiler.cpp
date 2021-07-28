/*
 * Copyright © 2017 Matt Robinson
 * Copyright © 2021 Mattias Jonsson
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "Boiler.h"
#include "FramingException.h"

const uint8_t COUNTER_EEPROM_BLOCK = 0x01;
const uint8_t COUNTER_BLOCK_COUNT  = 0x02;
const uint8_t DFDU_EEPROM_BLOCK    = 0x10;
const uint8_t DFDU_NUM_BYTES       = 0x02;

std::vector<uint8_t> Boiler::FetchData(FrameFunction function)
{
    int retries = 0;

    for(;;)
    {
        try
        {
            port.WriteBytes(Frame(FrameType::Request, function));
            return Frame(port.ReadBytes()).getData();
        }
        catch(FramingException&)
        {
            if(retries == 2)
            {
                throw;
            }

            retries++;
        }
    }
}

std::vector<uint8_t> Boiler::ReadEepromBlock(uint8_t blockNum)
{
    auto reply = FetchData((FrameFunction)(FrameFunction::EepromRead + blockNum));
    return reply;
}

std::vector<uint8_t> Boiler::ReadEepromBlocks(uint8_t blockNum, uint8_t count)
{
    std::vector<uint8_t> data;

    for(uint8_t i = blockNum; i < (blockNum + count); i++)
    {
        auto block = ReadEepromBlock(i);
        data.insert(data.end(), block.begin(), block.end());
    }

    return data;
}

IdentifyMessage Boiler::ReadIdentifyData()
{
    auto identData = FetchData(FrameFunction::Identify);
    auto dfDu = ReadEepromBlock(DFDU_EEPROM_BLOCK);

    // For some reason the 'dF-code' and 'dU-code' (whatever they are) aren't
    // included in the response to an identify request, but have to be read
    // out of a specific EEPROM block and tacked on to the end of the data
    identData.insert(identData.end(), dfDu.begin(), dfDu.begin() + DFDU_NUM_BYTES);
    return IdentifyMessage(identData);
}

SampleMessage Boiler::ReadSampleData()
{
    auto reply = FetchData(FrameFunction::Sample);
    return SampleMessage(reply);
}

CountersMessage Boiler::ReadCountersData()
{
    auto reply = ReadEepromBlocks(COUNTER_EEPROM_BLOCK, COUNTER_BLOCK_COUNT);
    return CountersMessage(reply);
}

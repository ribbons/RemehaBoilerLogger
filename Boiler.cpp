// Copyright © 2017 Matt Robinson

#include "Boiler.h"

const uint8_t DFDU_NUM_BYTES    = 0x02;
const uint8_t DFDU_EEPROM_BLOCK = 0x10;

std::vector<uint8_t> Boiler::FetchData(FrameFunction function)
{
    port.WriteBytes(Frame(FrameType::Request, function));
    return Frame(port.ReadBytes()).getData();
}

std::vector<uint8_t> Boiler::ReadEepromBlock(uint8_t blockNum)
{
    auto reply = FetchData((FrameFunction)(FrameFunction::EepromRead + blockNum));
    return reply;
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

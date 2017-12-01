// Copyright © 2017 Matt Robinson

#include "Boiler.h"

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
    auto reply = FetchData(FrameFunction::Identify);
    return IdentifyMessage(reply);
}

SampleMessage Boiler::ReadSampleData()
{
    auto reply = FetchData(FrameFunction::Sample);
    return SampleMessage(reply);
}

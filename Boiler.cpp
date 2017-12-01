// Copyright © 2017 Matt Robinson

#include "Boiler.h"

std::vector<uint8_t> Boiler::FetchData(FrameFunction function)
{
    port.WriteBytes(Frame(FrameType::Request, function));
    return Frame(port.ReadBytes()).getData();
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

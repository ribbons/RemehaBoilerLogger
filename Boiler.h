// Copyright © 2017 Matt Robinson

#include "Serial.h"
#include "Frame.h"
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
    IdentifyMessage ReadIdentifyData();
    SampleMessage ReadSampleData();
};

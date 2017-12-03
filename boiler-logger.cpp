// Copyright © 2017 Matt Robinson

#include <cstdlib>

#include "Boiler.h"
#include "Frame.h"

void dumpbytes(const char* label, std::vector<uint8_t> vals)
{
    printf("%s:\n", label);

    for(auto i : vals)
    {
        printf(" %02x", i);
    }

    fputc('\n', stdout);
}

int main(int argc, char* argv[])
{
    Boiler boiler("/dev/ttyUSB0");

    IdentifyMessage identify = boiler.ReadIdentifyData();

    for (auto& value : identify.getValues()) {
        printf("%s=%s\n", value.first.c_str(), value.second.c_str());
    }

    puts("\n");

    SampleMessage sample = boiler.ReadSampleData();

    for (auto& value : sample.getValues()) {
        printf("%s=%s\n", value.first.c_str(), value.second.c_str());
    }

    puts("\n");

    CountersMessage counters = boiler.ReadCountersData();

    for (auto& value : counters.getValues()) {
        printf("%s=%s\n", value.first.c_str(), value.second.c_str());
    }

    puts("\n");

    auto block = boiler.ReadEepromBlock(1);
    dumpbytes("EEPROM block", block);

    return EXIT_SUCCESS;
}

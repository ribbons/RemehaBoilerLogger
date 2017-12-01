// Copyright © 2017 Matt Robinson

#include <cstdlib>

#include "Boiler.h"
#include "Frame.h"

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

    return EXIT_SUCCESS;
}

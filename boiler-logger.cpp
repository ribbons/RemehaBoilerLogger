// Copyright © 2017 Matt Robinson

#include <cstdlib>

#include "Serial.h"
#include "Frame.h"
#include "IdentifyMessage.h"

int main(int argc, char* argv[])
{
    Serial port("/dev/ttyUSB0");

    port.WriteBytes(Frame(FrameType::Request, FrameFunction::Identify));
    auto identify_reply = port.ReadBytes();
    Message identMsg = Message::FromFrame(Frame(identify_reply));

    for (auto& value : identMsg.getValues()) {
        printf("%s=%s\n", value.first.c_str(), value.second.c_str());
    }

    puts("\n");

    port.WriteBytes(Frame(FrameType::Request, FrameFunction::Sample));
    auto sample_reply = port.ReadBytes();
    Message sampleMsg = Message::FromFrame(Frame(sample_reply));

    for (auto& value : sampleMsg.getValues()) {
        printf("%s=%s\n", value.first.c_str(), value.second.c_str());
    }

    return EXIT_SUCCESS;
}

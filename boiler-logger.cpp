// Copyright © 2017 Matt Robinson

#include <cstdlib>

#include "Serial.h"
#include "Frame.h"
#include "IdentifyMessage.h"

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
    Serial port("/dev/ttyUSB0");

    port.WriteBytes(Frame(FrameType::Request, FrameFunction::Identify));
    auto identify_reply = port.ReadBytes();
    Message msg = Message::FromFrame(Frame(identify_reply));

    for (auto& value : msg.getValues()) {
        printf("%s=%s\n", value.first.c_str(), value.second.c_str());
    }

    port.WriteBytes(Frame(FrameType::Request, FrameFunction::Status));
    auto status_reply = port.ReadBytes();
    dumpbytes("\nStatus payload", Frame(status_reply).getData());

    return EXIT_SUCCESS;
}

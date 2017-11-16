// Copyright © 2017 Matt Robinson

#include <cstdlib>

#include "Serial.h"
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
    Serial port("/dev/ttyUSB0");

    port.WriteBytes(Frame(FrameType::Request, 0x0B01));
    auto identify_reply = port.ReadBytes();
    dumpbytes("\nIdentify payload", Frame(identify_reply).getData());

    port.WriteBytes(Frame(FrameType::Request, 0x0002));
    auto status_reply = port.ReadBytes();
    dumpbytes("\nStatus payload", Frame(status_reply).getData());

    return EXIT_SUCCESS;
}

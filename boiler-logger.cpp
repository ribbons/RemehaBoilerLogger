// Copyright © 2017 Matt Robinson

#include <cstdlib>

#include "Serial.h"
#include "Frame.h"

static std::vector<uint8_t> req_status = { 0x02, 0x52, 0x05, 0x06, 0x02, 0x00, 0x53, 0x03 };

int main(int argc, char* argv[])
{
    Serial port("/dev/ttyUSB0");

    port.WriteBytes(req_status);
    auto reply = port.ReadBytes();

    Frame frame(reply);

    puts("Raw:");

    for(auto i : reply)
    {
        printf(" %02x", i);
    }

    puts("\n\nPayload:");

    for(auto i : frame.getData())
    {
        printf(" %02x", i);
    }

    fputc('\n', stdout);

    return EXIT_SUCCESS;
}

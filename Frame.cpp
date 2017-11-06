// Copyright © 2017 Matt Robinson

#include <cstring>
#include <stdexcept>

#include "Frame.h"

static const uint8_t START_BYTE = 0x02;
static const uint8_t END_BYTE   = 0x03;

static const int length_adjust = 2;

struct Header
{
    uint8_t start : 8;
    uint16_t : 16;
    uint8_t length : 8;
} __attribute__((__packed__));

struct Trailer
{
    uint8_t end : 8;
} __attribute__((__packed__));

Frame::Frame(const std::vector<uint8_t> &raw)
{
    if(raw.size() <= sizeof(Header) + sizeof(Trailer))
    {
        throw std::runtime_error("Not enough data for a valid frame");
    }

    struct Header header;
    memcpy(&header, &raw[0], sizeof(header));

    if(header.start != START_BYTE)
    {
        throw std::runtime_error("Incorrect start byte value");
    }

    if(header.length + length_adjust != raw.size())
    {
        throw std::runtime_error("Header length does not match actual length");
    }

    struct Trailer trailer;
    memcpy(&trailer, &raw[raw.size() - sizeof(trailer)], sizeof(trailer));

    if(trailer.end != END_BYTE)
    {
        throw std::runtime_error("Incorrect end byte value");
    }

    data = std::vector<uint8_t>(raw.begin() + sizeof(header), raw.begin() + (raw.size() - sizeof(trailer)));
}

std::vector<uint8_t> Frame::getData()
{
    return this->data;
}

/*
 * Copyright © 2017 Matt Robinson
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <cstring>

#include <arpa/inet.h>

#include "Frame.h"
#include "FramingException.h"

static const uint8_t START_BYTE = 0x02;
static const uint8_t END_BYTE   = 0x03;

static const uint LENGTH_ADJUST = 2;

struct Header
{
    uint8_t start : 8;
    FrameType type : 16;
    uint8_t length : 8;
    uint16_t function : 16;
} __attribute__((__packed__));

struct Trailer
{
    uint8_t csum : 8;
    uint8_t end : 8;
} __attribute__((__packed__));

Frame::Frame(const std::vector<uint8_t> &raw)
{
    if(raw.size() < sizeof(Header) + sizeof(Trailer))
    {
        throw FramingException("Not enough data for a valid frame");
    }

    struct Header header;
    memcpy(&header, &raw[0], sizeof(header));

    if(header.start != START_BYTE)
    {
        throw FramingException("Incorrect start byte value");
    }

    if(header.length + LENGTH_ADJUST != raw.size())
    {
        throw FramingException("Header length (" + std::to_string(header.length) + ") does not match actual length (" + std::to_string(raw.size() - LENGTH_ADJUST) + ")");
    }

    type = header.type;
    function = (FrameFunction)ntohs(header.function);

    struct Trailer trailer;
    memcpy(&trailer, &raw[raw.size() - sizeof(trailer)], sizeof(trailer));

    uint8_t csum = 0;

    for(uint i = 1; i < raw.size() - 2; i++)
    {
        csum = csum ^ raw[i];
    }

    if(trailer.csum != csum)
    {
        throw FramingException("Trailer checksum (" + std::to_string(trailer.csum) + ") does not match calculated (" + std::to_string(csum) + ")");
    }

    if(trailer.end != END_BYTE)
    {
        throw FramingException("Incorrect end byte value");
    }

    data = std::vector<uint8_t>(raw.begin() + sizeof(header), raw.begin() + (raw.size() - sizeof(trailer)));
}

Frame::operator std::vector<uint8_t>()
{
    std::vector<uint8_t> raw(sizeof(Header) + this->data.size() + sizeof(Trailer));

    struct Header header;
    header.start = START_BYTE;
    header.type = this->type;
    header.length = raw.size() - LENGTH_ADJUST;
    header.function = htons((uint16_t)this->function);

    memcpy(&raw[0], &header, sizeof(header));
    memcpy(&raw[sizeof(Header)], &this->data[0], this->data.size());

    uint8_t csum = 0;

    for(uint i = 1; i < raw.size() - 2; i++)
    {
        csum = csum ^ raw[i];
    }

    struct Trailer trailer;
    trailer.csum = csum;
    trailer.end = END_BYTE;
    memcpy(&raw[raw.size() - sizeof(trailer)], &trailer, sizeof(header));

    return raw;
}

FrameType Frame::getType()
{
    return this->type;
}

FrameFunction Frame::getFunction()
{
    return this->function;
}

std::vector<uint8_t> Frame::getData()
{
    return this->data;
}

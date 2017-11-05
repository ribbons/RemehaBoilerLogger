// Copyright © 2017 Matt Robinson

#include <stdexcept>

#include "Frame.h"

static const int MIN_FRAME_SIZE = 3;

static const uint8_t START_BYTE = 0x02;
static const uint8_t END_BYTE   = 0x03;

Frame::Frame(const std::vector<uint8_t> &raw)
{
    if(raw.size() < MIN_FRAME_SIZE)
    {
        throw std::runtime_error("Not enough data for a valid frame");
    }

    if(raw[0] != START_BYTE)
    {
        throw std::runtime_error("Incorrect start byte value");
    }

    if(raw.back() != END_BYTE)
    {
        throw std::runtime_error("Incorrect end byte value");
    }

    data = std::vector<uint8_t>(raw.begin() + 1, raw.begin() + (raw.size() - 1));
}

std::vector<uint8_t> Frame::getData()
{
    return this->data;
}

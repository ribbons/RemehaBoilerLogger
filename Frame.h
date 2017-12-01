// Copyright © 2017 Matt Robinson

#pragma once

#include <cstdint>
#include <vector>

enum FrameType : uint16_t
{
    Request = 0x0552,
    Reply   = 0x0641,
};

enum FrameFunction : uint16_t
{
    Identify   = 0x010B,
    Sample     = 0x0200,
    EepromRead = 0x1000,
};

class Frame
{
private:
    FrameType type;
    FrameFunction function;
    std::vector<uint8_t> data;

public:
    explicit Frame(FrameType type, FrameFunction function) : type(type), function(function) {}
    explicit Frame(const std::vector<uint8_t> &raw);

    operator std::vector<uint8_t>();

    FrameType getType();
    FrameFunction getFunction();
    std::vector<uint8_t> getData();
};

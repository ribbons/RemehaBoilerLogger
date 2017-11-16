// Copyright © 2017 Matt Robinson

#include <cstdint>
#include <vector>

enum FrameType : uint16_t
{
    Request = 0x0552,
    Reply   = 0x0641
};

class Frame
{
private:
    FrameType type;
    uint16_t function;
    std::vector<uint8_t> data;

public:
    explicit Frame(FrameType type, uint16_t function) : type(type), function(function) {}
    explicit Frame(const std::vector<uint8_t> &raw);

    operator std::vector<uint8_t>();

    FrameType getType();
    uint16_t getFunction();
    std::vector<uint8_t> getData();
};

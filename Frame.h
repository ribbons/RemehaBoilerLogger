// Copyright © 2017 Matt Robinson

#include <cstdint>
#include <vector>

enum FrameType : uint16_t
{
    Reply = 0x0641
};

class Frame
{
private:
    FrameType type;
    uint16_t function;
    std::vector<uint8_t> data;

public:
    explicit Frame(const std::vector<uint8_t> &raw);
    FrameType getType();
    uint16_t getFunction();
    std::vector<uint8_t> getData();
};

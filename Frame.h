// Copyright © 2017 Matt Robinson

#include <cstdint>
#include <vector>

class Frame
{
private:
    uint16_t function;
    std::vector<uint8_t> data;

public:
    explicit Frame(const std::vector<uint8_t> &raw);
    uint16_t getFunction();
    std::vector<uint8_t> getData();
};

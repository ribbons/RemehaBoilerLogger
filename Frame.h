// Copyright © 2017 Matt Robinson

#include <cstdint>
#include <vector>

class Frame
{
private:
    std::vector<uint8_t> data;

public:
    explicit Frame(const std::vector<uint8_t> &raw);
    std::vector<uint8_t> getData();
};

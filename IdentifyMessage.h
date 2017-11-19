// Copyright © 2017 Matt Robinson

#include <cstdint>
#include <map>
#include <vector>

class IdentifyMessage
{
private:
    std::map<std::string, std::string> values;

    std::string SpecialDataConv5(uint64_t data);
    std::string SpecialDataConv6(uint8_t data);

public:
    explicit IdentifyMessage(const std::vector<uint8_t> &raw);
    std::map<std::string, std::string> getValues();
};

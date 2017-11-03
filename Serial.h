// Copyright © 2017 Matt Robinson

#include <cstdint>
#include <string>
#include <vector>

#include <poll.h>

class Serial
{
private:
    int fd;
    struct pollfd fds[1];

public:
    explicit Serial(const std::string& path);
    ~Serial();
    std::vector<uint8_t> ReadBytes();
    void WriteBytes(const std::vector<uint8_t>& bytes);
};

/*
 * Copyright © 2017 Matt Robinson
 * Copyright © 2021 Mattias Jonsson
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <chrono>
#include <functional>
#include <string>

typedef std::function<void(const std::string&)> LogFunction;

class LogLimitValue
{
private:
    std::string name;
    std::string lastValue;
    std::chrono::time_point<std::chrono::steady_clock> started;
    long logCount;
    bool isNumeric;

    void WriteLog(const std::string &value);

public:
    static LogFunction LogFunc;

    explicit LogLimitValue(const std::string &name, const std::string &value);

    void NewValue(const std::string &value);
};

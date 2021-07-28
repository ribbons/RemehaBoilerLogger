/*
 * Copyright © 2017 Matt Robinson
 * Copyright © 2021 Mattias Jonsson
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <sstream>

#include "LogLimitValue.h"

const std::chrono::seconds MAX_NUM_LOG_FREQ(60);

LogFunction LogLimitValue::LogFunc;

void LogLimitValue::WriteLog(const std::string &value)
{
    std::ostringstream output;
    output << name << "=" << value;
    LogFunc(output.str());

    logCount++;
}

LogLimitValue::LogLimitValue(const std::string &name, const std::string &value) :
    name(name),
    lastValue(value),
    started(std::chrono::steady_clock::now()),
    logCount(0),
    // Names for numeric values have units in square brackets
    isNumeric(name.find('[') != std::string::npos)
{
    WriteLog(value);
};

void LogLimitValue::NewValue(const std::string &value)
{
    if(value == lastValue)
    {
        return;
    }

    if(isNumeric &&
       (std::chrono::steady_clock::now() - started) / logCount < MAX_NUM_LOG_FREQ)
    {
        return;
    }

    WriteLog(value);
    lastValue = value;
}

/*
 * Copyright © 2017-2021 Matt Robinson
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#pragma once

#include <cstdint>
#include <climits>
#include <map>
#include <string>
#include <vector>

class Message
{
protected:
    std::map<std::string, std::string> values;

    std::string SpecialDataConv5(uint64_t data);
    std::string SpecialDataConv6(uint8_t data);
    std::string FormatNumber(int value, float scale, int min = INT_MIN, int max = INT_MAX, std::string minText = {}, std::string maxText = {});
    std::string NoYesCodeString(uint8_t value);
    std::string StatusCodeString(uint8_t value);
    std::string LockingCodeString(uint8_t value);
    std::string BlockingCodeString(uint8_t value);
    std::string OffOnCodeString(uint8_t value);
    std::string OpenClosedCodeString(uint8_t value);
    std::string ClosedOpenCodeString(uint8_t value);

public:
    std::map<std::string, std::string> getValues();
};

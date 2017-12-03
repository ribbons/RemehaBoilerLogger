/*
 * This file is part of Remeha Boiler Logger.
 * Copyright © 2017 by the authors - see the AUTHORS file for details.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include <cstdint>
#include <climits>
#include <map>
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

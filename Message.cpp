/*
 * Copyright © 2017 Matt Robinson
 * Copyright © 2021 Mattias Jonsson
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <iomanip>
#include <sstream>

#include <arpa/inet.h>

#include "Message.h"

std::string Message::SpecialDataConv5(uint64_t data)
{
    uint16_t num1 =  data & 0x000000FFFF;
    char     lett = (data & 0x0000FF0000) >> 16;
    uint16_t num2 = (data & 0xFFFF000000) >> 24;

    std::stringstream stream;
    stream << std::hex << std::setfill('0') << std::setw(4) << ntohs(num1)
           << lett << std::setw(4) << ntohs(num2);

    return stream.str();
}

std::string Message::SpecialDataConv6(uint8_t data)
{
    uint hi = (data & 0xF0) >> 4;
    uint lo = data & 0x0F;

    std::stringstream stream;
    stream << std::hex << hi << '.' << lo;

    return stream.str();
}

std::string Message::FormatNumber(int value, float scale, int min, int max, std::string minText, std::string maxText)
{
    float scaled = (float)value * scale;

    if(scaled < min)
    {
        return minText;
    }
    else if(scaled > max)
    {
        return maxText;
    }

    std::stringstream stream;
    stream << scaled;
    return stream.str();
}

std::string Message::NoYesCodeString(uint8_t value)
{
    if(value == 0)
    {
        return "No";
    }
    else
    {
        return "Yes";
    }
}

std::string Message::StatusCodeString(uint8_t value)
{
    switch(value)
    {
        case 0:
            return "0: Off-position, no heat demand";
        case 1:
            return "1: Pre-vent./post-ventilation";
        case 2:
            return "2: Ignition";
        case 3:
            return "3: Running for CH";
        case 4:
            return "4: Running for DHW";
        case 5:
            return "5: Anti hunting time CH";
        case 6:
            return "6: Post pump time CH";
        case 7:
            return "7: Post pump time DHW";
        case 8:
            return "8: Control stop";
        case 9:
            return "9: Shut down";
        case 10:
            return "10: Venting cycle";
        case 11:
            return "11: Forced burner low flame";
        case 12:
            return "12: Forced burner high CH";
        case 13:
            return "13: Forced burner high DHW";
        case 15:
            return "15: Manual heat demand";
        default:
            return "Unknown State (" + std::to_string(value) + ")";
    }
}

std::string Message::LockingCodeString(uint8_t value)
{
    switch(value)
    {
        case 0:
            return "E00: Sensor error";
        case 1:
            return "E01: Flow > Tmax.";
        case 2:
            return "E02: Return > Flow";
        case 3:
            return "E03: Earthing fault";
        case 4:
            return "E04: No flame";
        case 5:
            return "E05: >5 start failures";
        case 6:
            return "E06: Flame simulation";
        case 7:
            return "E07: Water pressure too low";
        case 8:
            return "E08: Fan error";
        case 9:
            return "E09: Low hydraulic pressure";
        case 10:
            return "E10: Insufficient deaeration";
        case 11:
            return "E11: Airbox temp. > max.";
        case 12:
            return "E12: Error checking HRU";
        case 13:
            return "E13: Fuse";
        case 43:
            return "E43: Parameter bounds";
        case 44:
            return "E44: Parameter checksum";
        case 45:
            return "E45: Default parameters";
        case 46:
            return "E46: dF/dU table error";
        default:
            return "Unknown locking code (" + std::to_string(value) + ")";
    }
}

std::string Message::BlockingCodeString(uint8_t value)
{
    switch(value)
    {
        case 40:
            return "B40: Boiler codes unknown";
        case 80:
            return "B80: Shut down input";
        case 81:
            return "B81: Error checking HRU";
        case 82:
            return "B82: Solar boiler warm, DHW off";
        case 83:
            return "B83: Calorifier sensor error";
        case 90:
            return "B90: Temperaure increase";
        case 91:
            return "B91: Delta T to big";
        case 92:
            return "B92: Insufficient flow";
        default:
            return "Unknown blocking code (" + std::to_string(value) + ")";
    }
}

std::string Message::OffOnCodeString(uint8_t value)
{
    if(value == 0)
    {
        return "Off";
    }
    else
    {
        return "On";
    }
}

std::string Message::OpenClosedCodeString(uint8_t value)
{
    if(value == 0)
    {
        return "Open";
    }
    else
    {
        return "Closed";
    }
}

std::string Message::ClosedOpenCodeString(uint8_t value)
{
    return OpenClosedCodeString(!value);
}

std::map<std::string, std::string> Message::getValues()
{
    return this->values;
}

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

#include "LogLimitValue.h"

const std::chrono::seconds MAX_AVG_LOG_FREQ(30);

void LogLimitValue::WriteLog(const std::string &value)
{
    printf("%s=%s\n", name.c_str(), value.c_str());
    logCount++;
}

LogLimitValue::LogLimitValue(const std::string &name, const std::string &value) :
    name(name),
    lastValue(value),
    started(std::chrono::steady_clock::now()),
    logCount(0)
{
    WriteLog(value);
};

void LogLimitValue::NewValue(std::string value)
{
    if(value == lastValue)
    {
        return;
    }

    if((std::chrono::steady_clock::now() - started) / logCount < MAX_AVG_LOG_FREQ)
    {
        return;
    }

    WriteLog(value);
    lastValue = value;
}

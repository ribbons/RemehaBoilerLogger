/*
 * Copyright © 2017 Matt Robinson
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "LogLimit.h"

void LogLimit::NewValues(const std::map<std::string, std::string> &values)
{
    if(limitValues.empty())
    {
        for(auto& value : values)
        {
            limitValues.emplace(value.first, LogLimitValue(value.first, value.second));
        }
    }
    else
    {
        for(auto& value : values)
        {
            limitValues.at(value.first).NewValue(value.second);
        }
    }
}

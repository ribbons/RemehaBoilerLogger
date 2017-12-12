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

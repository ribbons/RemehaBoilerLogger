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

#include <cstdlib>
#include <thread>

#include "Boiler.h"

const std::chrono::hours IDENTIFY_INTERVAL(12);
const std::chrono::seconds SAMPLE_INTERVAL(1);
const std::chrono::minutes COUNTERS_INTERVAL(1);

using map = std::map<std::string, std::string>;

map lastIdentifyVals;
map lastCountersVals;
map lastSampleVals;

void writeDifferentVals(const map &newVals, map &oldVals)
{
    map filteredVals;

    if(oldVals.empty())
    {
        filteredVals = oldVals = newVals;
    }
    else
    {
        for(auto& newVal : newVals)
        {
            if(oldVals.at(newVal.first) != newVal.second)
            {
                filteredVals[newVal.first] = newVal.second;
            }
        }

        oldVals = newVals;
    }

    for(auto& value : filteredVals)
    {
        printf("%s=%s\n", value.first.c_str(), value.second.c_str());
    }
}

int main(int /* argc */, char* /* argv */[])
{
    Boiler boiler("/dev/ttyUSB0");

    std::chrono::time_point<std::chrono::steady_clock> lastIdentify;
    std::chrono::time_point<std::chrono::steady_clock> lastCounters;
    std::chrono::time_point<std::chrono::steady_clock> lastSample;

    while(true)
    {
        auto difference = std::chrono::steady_clock::now() - lastIdentify;

        if(difference >= IDENTIFY_INTERVAL)
        {
            lastIdentify = std::chrono::steady_clock::now();
            IdentifyMessage identify = boiler.ReadIdentifyData();
            writeDifferentVals(identify.getValues(), lastIdentifyVals);
        }

        difference = std::chrono::steady_clock::now() - lastCounters;

        if(difference >= COUNTERS_INTERVAL)
        {
            lastCounters = std::chrono::steady_clock::now();
            CountersMessage counters = boiler.ReadCountersData();
            writeDifferentVals(counters.getValues(), lastCountersVals);
        }

        difference = std::chrono::steady_clock::now() - lastSample;

        if(difference >= SAMPLE_INTERVAL)
        {
            lastSample = std::chrono::steady_clock::now();
            SampleMessage sample = boiler.ReadSampleData();
            writeDifferentVals(sample.getValues(), lastSampleVals);
        }

        std::this_thread::sleep_for(SAMPLE_INTERVAL);
    }

    return EXIT_SUCCESS;
}

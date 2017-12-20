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

#include <iostream>
#include <thread>

#include "Boiler.h"
#include "LogLimit.h"

const std::chrono::hours IDENTIFY_INTERVAL(12);
const std::chrono::milliseconds SAMPLE_INTERVAL(500);
const std::chrono::minutes COUNTERS_INTERVAL(1);

int main(int argc, char* argv[])
{
    if(argc != 2)
    {
        std::cerr << "Usage: boiler-logger <port>" << std::endl;
        return EXIT_FAILURE;
    }

    Boiler boiler(argv[1]);

    std::chrono::time_point<std::chrono::steady_clock> lastIdentify(-IDENTIFY_INTERVAL);
    std::chrono::time_point<std::chrono::steady_clock> lastCounters(-COUNTERS_INTERVAL);
    std::chrono::time_point<std::chrono::steady_clock> lastSample(-SAMPLE_INTERVAL);

    LogLimit trackIdentifyVals;
    LogLimit trackCountersVals;
    LogLimit trackSampleVals;

    while(true)
    {
        auto difference = std::chrono::steady_clock::now() - lastIdentify;

        if(difference >= IDENTIFY_INTERVAL)
        {
            lastIdentify = std::chrono::steady_clock::now();
            IdentifyMessage identify = boiler.ReadIdentifyData();
            trackIdentifyVals.NewValues(identify.getValues());
        }

        difference = std::chrono::steady_clock::now() - lastCounters;

        if(difference >= COUNTERS_INTERVAL)
        {
            lastCounters = std::chrono::steady_clock::now();
            CountersMessage counters = boiler.ReadCountersData();
            trackCountersVals.NewValues(counters.getValues());
        }

        difference = std::chrono::steady_clock::now() - lastSample;
        std::this_thread::sleep_for(SAMPLE_INTERVAL - difference);

        lastSample = std::chrono::steady_clock::now();
        SampleMessage sample = boiler.ReadSampleData();
        trackSampleVals.NewValues(sample.getValues());
    }

    return EXIT_SUCCESS;
}

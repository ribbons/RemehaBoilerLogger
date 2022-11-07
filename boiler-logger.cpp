/*
 * Copyright © 2017-2022 Matt Robinson
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <iostream>
#include <thread>

#include <syslog.h>

#include "Boiler.h"
#include "LogLimit.h"

const std::chrono::hours IDENTIFY_INTERVAL(12);
const std::chrono::milliseconds SAMPLE_INTERVAL(500);
const std::chrono::minutes COUNTERS_INTERVAL(1);

void logToStdout(const std::string &message)
{
    std::cout << message << std::endl;
}

void logToSyslog(const std::string &message)
{
    syslog(LOG_NOTICE, "%s", message.c_str());
}

int main(int argc, const char* const argv[])
{
    if(argc < 2 || argc > 3 || (argc == 3 && std::string(argv[1]) != "-d"))
    {
        std::cerr << "Usage: boiler-logger [-d] <port>" << std::endl;
        return EXIT_FAILURE;
    }

    if(argc == 3)
    {
        LogLimitValue::LogFunc = logToStdout;
    }
    else
    {
        LogLimitValue::LogFunc = logToSyslog;
        openlog(NULL, LOG_CONS | LOG_PID | LOG_NDELAY, LOG_USER);
    }

    Boiler boiler(argv[argc - 1]);

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

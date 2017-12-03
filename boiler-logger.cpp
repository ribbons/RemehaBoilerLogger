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

#include "Boiler.h"
#include "Frame.h"

void dumpbytes(const char* label, std::vector<uint8_t> vals)
{
    printf("%s:\n", label);

    for(auto i : vals)
    {
        printf(" %02x", i);
    }

    fputc('\n', stdout);
}

int main(int argc, char* argv[])
{
    Boiler boiler("/dev/ttyUSB0");

    IdentifyMessage identify = boiler.ReadIdentifyData();

    for (auto& value : identify.getValues()) {
        printf("%s=%s\n", value.first.c_str(), value.second.c_str());
    }

    puts("\n");

    SampleMessage sample = boiler.ReadSampleData();

    for (auto& value : sample.getValues()) {
        printf("%s=%s\n", value.first.c_str(), value.second.c_str());
    }

    puts("\n");

    CountersMessage counters = boiler.ReadCountersData();

    for (auto& value : counters.getValues()) {
        printf("%s=%s\n", value.first.c_str(), value.second.c_str());
    }

    puts("\n");

    auto block = boiler.ReadEepromBlock(1);
    dumpbytes("EEPROM block", block);

    return EXIT_SUCCESS;
}

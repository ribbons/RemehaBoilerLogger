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

#include <fstream>
#include <iostream>

#include "Boiler.h"

const uint8_t BLOCK_COUNT = 64;

int main(int argc, char* argv[])
{
    if(argc != 3)
    {
        std::cerr << "Usage: boiler-eeprom <port> <outfile>" << std::endl;
        return EXIT_FAILURE;
    }

    std::string port = argv[1];
    std::string filename = argv[2];

    std::cout << "Connecting to boiler at " << port << "... " << std::flush;
    Boiler boiler(port);
    boiler.ReadIdentifyData();
    std::cout << "done" << std::endl;

    std::ofstream out(filename, std::ios::out | std::ios::binary);
    std::cout << "Dumping boiler EEPROM to " << filename << std::flush;

    for(uint8_t i = 0; i < BLOCK_COUNT; i++)
    {
        auto eeprom = boiler.ReadEepromBlock(i);
        out.write((const char*)&eeprom[0], eeprom.size());

        std::cout << "." << std::flush;
    }

    std::cout << " done" << std::endl;

    out.close();

    return EXIT_SUCCESS;
}

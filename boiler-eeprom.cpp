/*
 * Copyright © 2017 Matt Robinson
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
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

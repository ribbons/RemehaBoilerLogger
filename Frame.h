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

#pragma once

#include <cstdint>
#include <vector>

enum FrameType : uint16_t
{
    Request = 0x0552,
    Reply   = 0x0641,
};

enum FrameFunction : uint16_t
{
    Identify   = 0x010B,
    Sample     = 0x0200,
    EepromRead = 0x1000,
};

class Frame
{
private:
    FrameType type;
    FrameFunction function;
    std::vector<uint8_t> data;

public:
    explicit Frame(FrameType type, FrameFunction function) : type(type), function(function) {}
    explicit Frame(const std::vector<uint8_t> &raw);

    operator std::vector<uint8_t>();

    FrameType getType();
    FrameFunction getFunction();
    std::vector<uint8_t> getData();
};

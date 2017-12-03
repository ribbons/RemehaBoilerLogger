# This file is part of Remeha Boiler Logger.
# Copyright © 2017 by the authors - see the AUTHORS file for details.
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

cmake_minimum_required(VERSION 3.2)
project("boiler")

set(Boost_USE_STATIC_LIBS ON)
find_package(Boost REQUIRED COMPONENTS unit_test_framework)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})

add_executable(boiler-logger Serial.cpp Boiler.cpp Frame.cpp Message.cpp
               IdentifyMessage.cpp SampleMessage.cpp CountersMessage.cpp
               boiler-logger.cpp)
set_property(TARGET boiler-logger PROPERTY CXX_STANDARD 11)
set_property(TARGET boiler-logger PROPERTY CXX_STANDARD_REQUIRED ON)

add_executable(tests Frame.cpp Message.cpp IdentifyMessage.cpp
                     SampleMessage.cpp CountersMessage.cpp tests.cpp)
set_property(TARGET tests PROPERTY CXX_STANDARD 11)
set_property(TARGET tests PROPERTY CXX_STANDARD_REQUIRED ON)
TARGET_LINK_LIBRARIES(tests ${Boost_LIBRARIES})
add_custom_command(TARGET tests POST_BUILD COMMAND tests)

file(GLOB CPP_FILES *.cpp)

add_custom_target(cppcheck ALL COMMAND cppcheck --quiet --error-exitcode=2
                  --enable=all --suppress=missingIncludeSystem --std=c++11
                  --inline-suppr ${CPP_FILES}
                  DEPENDS boiler-logger VERBATIM)

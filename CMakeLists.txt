# Copyright © 2017-2022 Matt Robinson
#
# SPDX-License-Identifier: GPL-3.0-or-later

cmake_minimum_required(VERSION 3.2)
project("boiler")

set(Boost_USE_STATIC_LIBS ON)
find_package(Boost REQUIRED COMPONENTS unit_test_framework)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})

if(CMAKE_COMPILER_IS_GNUCXX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror")
endif()

add_library(shared OBJECT Frame.cpp Message.cpp IdentifyMessage.cpp
            SampleMessage.cpp CountersMessage.cpp)

add_library(boiler-shared OBJECT Serial.cpp Boiler.cpp)

add_executable(boiler-logger LogLimit.cpp
               LogLimitValue.cpp boiler-logger.cpp
               $<TARGET_OBJECTS:shared> $<TARGET_OBJECTS:boiler-shared>)

add_executable(boiler-eeprom boiler-eeprom.cpp
               $<TARGET_OBJECTS:shared> $<TARGET_OBJECTS:boiler-shared>)

add_executable(tests tests.cpp $<TARGET_OBJECTS:shared>)
TARGET_LINK_LIBRARIES(tests ${Boost_LIBRARIES})
add_custom_command(TARGET tests POST_BUILD COMMAND tests)

set_target_properties(shared boiler-shared boiler-logger boiler-eeprom tests
                      PROPERTIES CXX_STANDARD 11
                                 CXX_STANDARD_REQUIRED ON)

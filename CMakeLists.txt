# Copyright © 2017 Matt Robinson

cmake_minimum_required(VERSION 3.2)
project("boiler")

set(Boost_USE_STATIC_LIBS ON)
find_package(Boost REQUIRED COMPONENTS unit_test_framework)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})

add_executable(boiler-logger Serial.cpp Boiler.cpp Frame.cpp Message.cpp
               IdentifyMessage.cpp SampleMessage.cpp boiler-logger.cpp)
set_property(TARGET boiler-logger PROPERTY CXX_STANDARD 11)
set_property(TARGET boiler-logger PROPERTY CXX_STANDARD_REQUIRED ON)

add_executable(tests Frame.cpp Message.cpp IdentifyMessage.cpp
                     SampleMessage.cpp tests.cpp)
set_property(TARGET tests PROPERTY CXX_STANDARD 11)
set_property(TARGET tests PROPERTY CXX_STANDARD_REQUIRED ON)
TARGET_LINK_LIBRARIES(tests ${Boost_LIBRARIES})
add_custom_command(TARGET tests POST_BUILD COMMAND tests)

file(GLOB CPP_FILES *.cpp)

add_custom_target(cppcheck ALL COMMAND cppcheck --quiet --error-exitcode=2
                  --enable=all --suppress=missingIncludeSystem --std=c++11
                  --inline-suppr ${CPP_FILES}
                  DEPENDS boiler-logger VERBATIM)

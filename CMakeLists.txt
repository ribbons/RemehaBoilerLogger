# Copyright © 2017 Matt Robinson

cmake_minimum_required(VERSION 3.2)
project("boiler")

add_executable(boiler-logger Serial.cpp boiler-logger.cpp)
set_property(TARGET boiler-logger PROPERTY CXX_STANDARD 11)
set_property(TARGET boiler-logger PROPERTY CXX_STANDARD_REQUIRED ON)

file(GLOB CPP_FILES *.cpp)

add_custom_target(cppcheck ALL COMMAND cppcheck --quiet --error-exitcode=2
                  --enable=all --suppress=missingIncludeSystem --std=c++11
                  ${CPP_FILES}
                  DEPENDS boiler-logger VERBATIM)

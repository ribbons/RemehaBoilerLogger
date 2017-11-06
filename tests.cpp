// Copyright © 2017 Matt Robinson

#include <stdexcept>

#include "Frame.h"

#define BOOST_TEST_MODULE Boiler test
#include <boost/test/unit_test.hpp>

void create_frame(std::vector<uint8_t> data)
{
    Frame frame(data);
}

BOOST_AUTO_TEST_CASE(frame_no_data)
{
    BOOST_CHECK_THROW(create_frame({}), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(frame_wrong_start)
{
    BOOST_CHECK_THROW(create_frame({ 0x0, 0x0, 0x0, 0x4, 0x0, 0x3 }), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(frame_wrong_end)
{
    BOOST_CHECK_THROW(create_frame({ 0x2, 0x0, 0x0, 0x4, 0x0, 0x0 }), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(frame_wrong_length)
{
    BOOST_CHECK_THROW(create_frame({ 0x2, 0x0, 0x0, 0xF, 0x0, 0x0 }), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(frame_valid)
{
    BOOST_CHECK_NO_THROW(create_frame({ 0x2, 0x0, 0x0, 0x4, 0x0, 0x3 }));
}

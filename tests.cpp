// Copyright © 2017 Matt Robinson

#include <stdexcept>

#include "Frame.h"
#include "FramingException.h"

#define BOOST_TEST_MODULE Boiler test
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(frame_no_data)
{
    BOOST_CHECK_THROW(Frame frame({}), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(frame_wrong_start)
{
    BOOST_CHECK_THROW(Frame frame({ 0x0, 0x0, 0x0, 0x6, 0x0, 0x0, 0x6, 0x3 }), FramingException);
}

BOOST_AUTO_TEST_CASE(frame_wrong_end)
{
    BOOST_CHECK_THROW(Frame frame({ 0x2, 0x0, 0x0, 0x6, 0x0, 0x0, 0x6, 0x0 }), FramingException);
}

BOOST_AUTO_TEST_CASE(frame_wrong_length)
{
    BOOST_CHECK_THROW(Frame frame({ 0x2, 0x0, 0x0, 0xF, 0x0, 0x0, 0xF, 0x3 }), FramingException);
}

BOOST_AUTO_TEST_CASE(frame_wrong_csum)
{
    BOOST_CHECK_THROW(Frame frame({ 0x2, 0x0, 0x0, 0x6, 0x0, 0x0, 0xF, 0x3 }), FramingException);
}

BOOST_AUTO_TEST_CASE(frame_function_code)
{
    Frame frame({ 0x02, 0x00, 0x00, 0x06, 0x34, 0x12, 0x20, 0x03 });
    BOOST_CHECK_EQUAL(frame.getFunction(), 0x1234);
}

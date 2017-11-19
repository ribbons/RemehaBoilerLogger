// Copyright © 2017 Matt Robinson

#include <stdexcept>

#include "Frame.h"
#include "FramingException.h"
#include "IdentifyMessage.h"

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

BOOST_AUTO_TEST_CASE(frame_type)
{
    Frame frame({ 0x02, 0x41, 0x06, 0x06, 0x00, 0x00, 0x41, 0x03 });
    BOOST_CHECK_EQUAL(frame.getType(), FrameType::Reply);
}

BOOST_AUTO_TEST_CASE(frame_function_code)
{
    Frame frame({ 0x02, 0x00, 0x00, 0x06, 0x34, 0x12, 0x20, 0x03 });
    BOOST_CHECK_EQUAL(frame.getFunction(), 0x1234);
}

BOOST_AUTO_TEST_CASE(frame_data)
{
    Frame frame({ 0x02, 0x00, 0x00, 0x07, 0x00, 0x00, 0x12, 0x15, 0x03 });

    auto data = frame.getData();
    std::vector<uint8_t> compare({ 0x12 });

    BOOST_CHECK_EQUAL_COLLECTIONS(data.begin(), data.end(),
                                  compare.begin(), compare.end());
}

BOOST_AUTO_TEST_CASE(frame_to_raw)
{
    Frame frame(FrameType::Request, (FrameFunction)0x1234);

    std::vector<uint8_t> raw = frame;
    std::vector<uint8_t> compare({ 0x02, 0x52, 0x05, 0x06, 0x34, 0x12, 0x77, 0x03 });

    BOOST_CHECK_EQUAL_COLLECTIONS(raw.begin(), raw.end(),
                                  compare.begin(), compare.end());
}

BOOST_AUTO_TEST_CASE(identify_message)
{
    Frame frame(FrameType::Request, FrameFunction::Identify);

    frame.setData(std::vector<uint8_t>(
        { 0x07, 0x00, 0x00, 0x14, 0x20, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00,
          0x12, 0x34, 0x4D, 0x67, 0x89 }));

    Message msg = Message::FromFrame(frame);

    std::map<std::string, std::string> compare = {
        {"Parameter type",    "2"},
        {"Parameter version", "2.0"},
        {"Serial number",     "1234M6789"},
        {"Software version",  "1.4"}
    };

    BOOST_CHECK(msg.getValues() == compare);
}

// Copyright © 2017 Matt Robinson

#include <stdexcept>

#include "Frame.h"
#include "FramingException.h"
#include "IdentifyMessage.h"
#include "SampleMessage.h"

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
    std::vector<uint8_t> data(
        { 0x07, 0x00, 0x00, 0x14, 0x20, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00,
          0x12, 0x34, 0x4D, 0x67, 0x89 });

    IdentifyMessage msg(data);

    std::map<std::string, std::string> compare = {
        {"Parameter type",    "2"},
        {"Parameter version", "2.0"},
        {"Serial number",     "1234M6789"},
        {"Software version",  "1.4"}
    };

    BOOST_CHECK(msg.getValues() == compare);
}

BOOST_AUTO_TEST_CASE(sample_message)
{
    std::vector<uint8_t> data(
        { 0x88, 0x13, 0x6b, 0x12, 0x80, 0xf3, 0x80, 0xf3, 0x80, 0xf3, 0x80,
          0xf3, 0x66, 0x12, 0x00, 0x80, 0x64, 0x19, 0x7c, 0x15, 0x00, 0x80,
          0x8b, 0x05, 0xa6, 0x05, 0x1b, 0x28, 0x23, 0x00, 0x32, 0x00, 0x64,
          0x06, 0x00, 0x00, 0x04, 0xc5, 0x00, 0x01, 0x03, 0xff, 0xff, 0x00,
          0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
          0x00 });

    SampleMessage msg(data);

    std::map<std::string, std::string> compare = {
        {"Actual power [%]",          "6"},
        {"Anti Legionella",           "No"},
        {"Available power [%]",       "0"},
        {"Boiler Control temp. [°C]", "47.1"},
        {"CH Setpoint [°C]",          "65"},
        {"CH enable",                 "Closed"},
        {"Calorifier temp. [°C]",     "Open"},
        {"Calorifier thermostat",     "Open"},
        {"DHW Setpoint [°C]",         "55"},
        {"DHW eco",                   "Yes"},
        {"DHW enable",                "Closed"},
        {"DHW heat demand",           "No"},
        {"DHW temp. [°C]",            "Open"},
        {"Fan speed [Rpm]",           "1446"},
        {"Fanspeed setpoint [Rpm]",   "1419"},
        {"Flow switch",               "Off"},
        {"Flow temp. [°C]",           "50"},
        {"Gas valve",                 "Open"},
        {"Ignition",                  "Off"},
        {"Internal setpoint [°C]",    "90"},
        {"Ionisation",                "Yes"},
        {"Ionisation current [uA]",   "2.7"},
        {"Lock-out",                  "Unknown locking code"},
        {"Mod. heat demand",          "No"},
        {"On/off heat demand",        "Yes"},
        {"Outside temp. [°C]",        "Open"},
        {"Pump",                      "On"},
        {"Pump [%]",                  "50"},
        {"Required output [%]",       "100"},
        {"Return temp. [°C]",         "47.15"},
        {"Room temp setpoint [°C]",   "Open"},
        {"Room temp. [°C]",           "Open"},
        {"Shut down input",           "Closed"},
        {"Shutdown",                  "Unknown blocking code"},
        {"State",                     "3: Running for CH"},
        {"Status report",             "Open"},
        {"Three way valve",           "Off"}
    };

    BOOST_CHECK(msg.getValues() == compare);
}

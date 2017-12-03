// Copyright © 2017 Matt Robinson

#include <cstring>

#include "SampleMessage.h"

struct sample
{
    int16_t f4 : 16;
    int16_t f5 : 16;
    int16_t f6 : 16;
    int16_t f8 : 16;
    int16_t f7 : 16;
    uint16_t : 16;
    int16_t f14 : 16;
    int16_t f12 : 16;
    int16_t f10 : 16;
    int16_t f11 : 16;
    int16_t f13 : 16;
    uint16_t f15 : 16;
    uint16_t f16 : 16;
    uint8_t f17 : 8;
    int16_t f9 : 16;
    uint8_t f19 : 8;
    uint8_t f18 : 8;
    uint8_t : 8;
    uint8_t f21 : 8;
    uint8_t f20 : 8;
    uint32_t : 17;
    bool f27 : 1;
    bool f28 : 1;
    uint8_t : 1;
    bool f29 : 1;
    uint8_t : 1;
    bool f31 : 1;
    bool f30 : 1;
    bool f33 : 1;
    bool f34 : 1;
    bool f24 : 1;
    bool f32 : 1;
    uint8_t : 2;
    bool f36 : 1;
    bool f37 : 1;
    bool f22 : 1;
    uint8_t : 1;
    bool f23 : 1;
    bool f25 : 1;
    uint8_t : 4;
    bool f26 : 1;
    uint8_t : 3;
    bool f35 : 1;
    uint8_t : 3;
    uint8_t f1 : 8;
    uint8_t f2 : 8;
    uint8_t f3 : 8;
} __attribute__((__packed__));

SampleMessage::SampleMessage(const std::vector<uint8_t> &raw)
{
    if(raw.size() < sizeof(sample))
    {
        throw std::runtime_error("Not enough data!");
    }

    struct sample unpack;
    memcpy(&unpack, &raw[0], sizeof(unpack));

    this->values["Flow temp. [°C]"] = FormatNumber(unpack.f4, 0.01, -25, 150, "Open", "Closed");
    this->values["Return temp. [°C]"] = FormatNumber(unpack.f5, 0.01, -25, 150, "Open", "Closed");
    this->values["DHW temp. [°C]"] = FormatNumber(unpack.f6, 0.01, -25, 150, "Open", "Closed");
    this->values["Outside temp. [°C]"] = FormatNumber(unpack.f8, 0.01, -25, 150, "Open", "Closed");
    this->values["Calorifier temp. [°C]"] = FormatNumber(unpack.f7, 0.01, -25, 150, "Open", "Closed");
    this->values["Boiler Control temp. [°C]"] = FormatNumber(unpack.f14, 0.01, -25, 150, "Open", "Closed");
    this->values["Room temp. [°C]"] = FormatNumber(unpack.f12, 0.01, -25, 150, "Open", "Closed");
    this->values["CH Setpoint [°C]"] = FormatNumber(unpack.f10, 0.01, 0, 100, "Open", "Closed");
    this->values["DHW Setpoint [°C]"] = FormatNumber(unpack.f11, 0.01, 0, 100, "Open", "Closed");
    this->values["Room temp setpoint [°C]"] = FormatNumber(unpack.f13, 0.01, 0, 100, "Open", "Closed");
    this->values["Fanspeed setpoint [Rpm]"] = FormatNumber(unpack.f15, 1, 0, 7000, "0", "7000");
    this->values["Fan speed [Rpm]"] = FormatNumber(unpack.f16, 1, 0, 7000, "0", "7000");
    this->values["Ionisation current [uA]"] = FormatNumber(unpack.f17, 0.1, 0, 10, "0", "10");
    this->values["Internal setpoint [°C]"] = FormatNumber(unpack.f9, 0.01, 0, 100, "Open", "Closed");
    this->values["Available power [%]"] = FormatNumber(unpack.f19, 1, 0, 100, "0", "100");
    this->values["Pump [%]"] = FormatNumber(unpack.f18, 1, 0, 100, "0", "100");
    this->values["Required output [%]"] = FormatNumber(unpack.f21, 1, 0, 100, "0", "100");
    this->values["Actual power [%]"] = FormatNumber(unpack.f20, 1, 0, 100, "0", "100");
    this->values["Mod. heat demand"] = NoYesCodeString(unpack.f27);
    this->values["On/off heat demand"] = NoYesCodeString(unpack.f28);
    this->values["DHW eco"] = NoYesCodeString(!unpack.f29);
    this->values["Anti Legionella"] = NoYesCodeString(unpack.f31);
    this->values["DHW heat demand"] = NoYesCodeString(unpack.f30);
    this->values["Shut down input"] = OpenClosedCodeString(unpack.f33);
    this->values["Calorifier thermostat"] = OpenClosedCodeString(unpack.f34);
    this->values["Ionisation"] = NoYesCodeString(unpack.f24);
    this->values["Flow switch"] = OffOnCodeString(unpack.f32);
    this->values["CH enable"] = OpenClosedCodeString(unpack.f36);
    this->values["DHW enable"] = OpenClosedCodeString(unpack.f37);
    this->values["Gas valve"] = ClosedOpenCodeString(!unpack.f22);
    this->values["Ignition"] = OffOnCodeString(unpack.f23);
    this->values["Three way valve"] = OffOnCodeString(unpack.f25);
    this->values["Pump"] = OffOnCodeString(unpack.f26);
    this->values["Status report"] = OpenClosedCodeString(unpack.f35);
    this->values["State"] = StatusCodeString(unpack.f1);
    this->values["Lock-out"] = LockingCodeString(unpack.f2);
    this->values["Shutdown"] = BlockingCodeString(unpack.f3);
}

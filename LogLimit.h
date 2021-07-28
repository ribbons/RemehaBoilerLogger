/*
 * Copyright © 2017 Matt Robinson
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <map>

#include "LogLimitValue.h"

class LogLimit
{
private:
    std::map<std::string, LogLimitValue> limitValues;

public:
    void NewValues(const std::map<std::string, std::string> &values);
};

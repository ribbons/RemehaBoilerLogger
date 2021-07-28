/*
 * Copyright © 2017 Matt Robinson
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <stdexcept>

class FramingException: public std::runtime_error
{
    using std::runtime_error::runtime_error;
};

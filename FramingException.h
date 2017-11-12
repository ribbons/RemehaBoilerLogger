// Copyright © 2017 Matt Robinson

#include <stdexcept>

class FramingException: public std::runtime_error
{
    using std::runtime_error::runtime_error;
};

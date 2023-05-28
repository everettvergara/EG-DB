#pragma once

#include "common.hpp"

namespace eg
{
    struct column_attr
    {
        const char *name;
        uint_t max_size;
        bool is_varsize;
    };
}


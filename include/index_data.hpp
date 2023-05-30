#pragma once

#include "common.hpp"

namespace eg
{
    struct index_data
    {
        uint_t pos;
        uint_t size;
        bool   active;

        index_data(const uint_t p, const uint_t s, const uint_t a)
            : pos(p), size(s), active(a)
        {
        }
    };
}
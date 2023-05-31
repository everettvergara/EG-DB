#pragma once

#include "common.hpp"

namespace eg
{

    struct index_data
    {
        uint_t pos;
        uint_t size;
        uint_t next;
        uint_t prev;

        index_data()
            : pos(0), size(0), next(0), prev(0)
        {
        }

        index_data(const uint_t p, const uint_t s, const uint_t nx, const uint_t pv)
            : pos(p), size(s), next(nx), prev(pv)
        {
        }
    };

    struct full_index_data
    {
        uint_t      ix;
        index_data  data;

        full_index_data()
            : ix(0)
        {
        }

        full_index_data(const uint_t i, const index_data d)
            : ix(i), data(d)
        {
        }
    };
}
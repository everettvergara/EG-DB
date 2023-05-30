#pragma once

#include "common.hpp"

namespace eg
{
    /*
        optimize by removing active and adding prev and next.

        implement index_data as list and at the same time a hash map
    
    */
    struct index_data
    {
        uint_t pos;
        uint_t size;
        bool   active;

        index_data()
            : pos(0), size(0), active(false)
        {
        }

        index_data(const uint_t p, const uint_t s, const uint_t a)
            : pos(p), size(s), active(a)
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
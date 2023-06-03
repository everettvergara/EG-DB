#pragma once

#include <vector>
#include <string>
#include <filesystem>
#include <memory>

#include "common.hpp"
#include "block_data.hpp"

namespace eg
{
    /*
    struct inactive_alloc : std::allocator<uint16_t>
    {
        auto allocate(size_t n) -> uint16_t *
        {
            auto *p = std::allocator<uint16_t>::allocate(n);

            for (size_t i = 0; i < 0xffff; ++i)
                p[i] = static_cast<uint16_t>(i);

            return p;
        }

        auto deallocate(uint16_t *p, size_t n)
        {
            std::allocator<uint16_t>::deallocate(p, n);
        }
    };
    */

    enum struct page_data_status : uint8_t
    {
        active, inactive, deleted, locked
    };


    // Recommended starting size: 256 records
    template <uint16_t S>
    struct page
    {
        page_data_status    status[S];
        uint16_t            active[S];
        uint16_t            inactive[S];
        uint16_t            active_size;
        uint16_t            inactive_size;

        auto init()
        {
            // Set status
            for (uint32_t i = 0; i < S; ++i)
                status[i] = page_data_status::inactive;

            // Set actives
            active_size = 0;

            // Set inactives;
            inactive_size = S;
            for (uint32_t i = 0; i < S; ++i)
                inactive[i] = S - i - 1;
        }

        auto save(const std::string &filename, const uint_t i)
        {
            write_block_data<page>(filename, *this, i);
        }

        auto load(const std::string &filename, const uint_t i)
        {
            *this = read_block_data<page>(filename, i);
        }
    };

/*
    struct page_master_data
    {
        uint_t rows_per_page_;
        uint_t last_page_;
    };

    class page_master
    {
    public:

        page_master(const std::string &table_name)
            : page_master_file_(table_name + ".pm")
        {
        }

        auto init_page_master()
        {
        }

        auto get_page_no(const uint_t r) -> uint_t
        {
        }



    private:
        std::string         page_master_file_;
        page_master_data    data_;

    };

*/
}
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
        uint8_t     status[S];
        uint16_t    active[S];
        uint16_t    inactive[S];
        uint16_t    active_size;
        uint16_t    inactive_size;

        
    };

/*
    struct page_data
    {
        std::vector<page_data_status> status;
        
        std::vector<uint16_t>   active;
        uint16_t                active_size;

        std::vector<uint16_t>   inactive;
        uint16_t                inactive_size;

        auto init_page_data()
        {
            size_t size = sizeof(uint16_t) * 8;

            status.resize(size, page_data_status::inactive);

            active.resize(size);
            active_size = 0;

            // Inactive contains 0xffff to 0;
            inactive.reserve(size);
            for (size_t i = 0xfffe; i >=0 size; --i)
                inactive.emplace_back(i);
            inactive_size = inactive.size();
        }

        page_data()
        {
        }
    };


    class page
    {
    public:

    private:
        page_data data_;
    };



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
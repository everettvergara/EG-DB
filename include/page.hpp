#pragma once

#include <string>
#include <filesystem>
#include "common.hpp"

namespace eg
{

    struct page_data
    {

    };

    struct page
    {

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
}
#pragma once

#include <string>
#include <filesystem>
#include "common.hpp"

namespace eg
{

    class page_master
    {
    public:

        page_master(const std::string &table_name)
            : page_master_file_(table_name + ".pm")
        {
        }

    private:
        std::string page_master_file_;
        uint_t rows_per_page_;
        uint_t last_page_;
    };
}
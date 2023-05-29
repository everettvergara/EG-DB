#pragma once

#include <string>
#include <fstream>
#include "common.hpp"

namespace eg
{

    struct index_data
    {
        uint_t ix;
        uint_t size;
    };

    class index
    {
    public:

        index(const std::string &table_name)
            : index_file_(table_name + ".ix")
        {
            index::init_index(index_file_);
        }

        auto create(const uint_t i, const uint_t s)
        {
        }

    private:

        const std::string index_file_;

        static auto init_index(const std::string &index_file) -> void
        {
            std::ofstream file(index_file, std::ios::binary | std::ios::app);

            // Check if index file is writable
            if (not file) std::runtime_error("Unable to open the index file.");

            // Check if the file has the right size
            file.seekp(0, std::ios::end);
            
            // Check if .id file does not exists
            auto size = file.tellp();
            if (size % sizeof(index_data) not_eq 0)
            {
                std::runtime_error("The existing index file is incompatible with this version.");
            }
        }

    };

}
#pragma once

#include <string>
#include <fstream>
#include <tuple>
#include "common.hpp"

namespace eg
{

    struct index_data
    {
        uint_t ix;
        uint_t pos;
        uint_t size;
        bool   active;
    };

    class index
    {
    public:

        index(const std::string &table_name)
            : index_file_(table_name + ".ix")
        {
            index::init_index(index_file_);
        }

        auto write(const uint_t i, const uint_t p, const uint_t s, const bool a = 1)
        {
            std::ofstream file(index_file_, std::ios::binary);

            // Check if index file is writable
            if (not file) std::runtime_error("Unable to open the index file.");

            // Check if the file has the right size
            std::streampos pos = sizeof(index_data) * i;
            file.seekp(pos, std::ios::beg);

            // Create IX record
            index_data data {.ix = i, .pos = p, .size = s, .active = a};
            file.write(reinterpret_cast<char *>(&data), sizeof(index_data));
            if (file.fail()) std::runtime_error("Unable to write to index file.");
        }

        auto read(const uint_t i) -> std::tuple<uint_t, uint_t, bool>
        {
            std::ifstream file(index_file_, std::ios::binary);

            // Check if index file is readable
            if (not file) std::runtime_error("Unable to open the index file.");

            // Check if the file has the right size
            std::streampos pos = sizeof(index_data) * i;
            file.seekg(pos, std::ios::beg);

            index_data data;
            file.read(reinterpret_cast<char *>(&data), sizeof(index_data));
            if (file.fail()) std::runtime_error("Unable to read from index file.");

            return {data.pos, data.size, data.active};
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
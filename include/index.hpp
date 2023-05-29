#pragma once

#include <string>
#include <fstream>
#include <vector>

#include "common.hpp"
#include "index_data.hpp"

namespace eg
{
    class index
    {
    public:

        index(const std::string &table_name)
            : index_file_(table_name + ".ix")
        {
            index::init_index(index_file_);
        }

        auto write(const uint_t i, const index_data &data)
        {
            std::ofstream file(index_file_, std::ios::binary);

            // Check if index file is writable
            if (not file) throw std::runtime_error("Unable to open the index file.");

            // Check if the file has the right size
            std::streampos pos = sizeof(index_data) * i;
            file.seekp(pos, std::ios::beg);

            // Create IX record
            std::cout << "write: " << data.pos << " " << data.size << " " << data.active << std::endl;
            file.write(reinterpret_cast<const char *>(&data), sizeof(index_data));
            
            if (file.fail()) throw std::runtime_error("Unable to write to index file.");
        }

        auto read(const uint_t i) -> index_data
        {
            std::ifstream file(index_file_, std::ios::binary);

            // Check if index file is readable
            if (not file) throw std::runtime_error("Unable to open the index file.");

            // Check if the file has the right size
            std::streampos pos = sizeof(index_data) * i;
            file.seekg(pos, std::ios::beg);

            index_data data;
            file.read(reinterpret_cast<char *>(&data), sizeof(index_data));
            if (file.fail()) throw std::runtime_error("Unable to read from index file.");

            return data;
        }


        auto read_range(const uint_t i, const uint_t j) -> std::vector<index_data>
        {
            std::ifstream file(index_file_, std::ios::binary);

            // Check if index file is readable
            if (not file) throw std::runtime_error("Unable to open the index file.");

            // Check if the file has the right size
            std::streampos pos = sizeof(index_data) * i;
            file.seekg(pos, std::ios::beg);

            std::streampos pos_end = sizeof(index_data) * (j + 1);
            std::streampos to_read = pos_end - pos;

            std::vector<index_data> data;
            data.resize(j - i + 1);
            file.read(reinterpret_cast<char *>(data.data()), to_read);
            if (file.fail()) throw std::runtime_error("Unable to read from index file.");

            return data;
        }

    private:

        const std::string index_file_;

        static auto init_index(const std::string &index_file) -> void
        {
            std::ofstream file(index_file, std::ios::binary | std::ios::app);

            // Check if index file is writable
            if (not file) throw std::runtime_error("Unable to open the index file.");

            // Check if the file has the right size
            file.seekp(0, std::ios::end);
            
            // Check if .id file does not exists
            if (auto size = file.tellp(); size == 0)
            {
                index_data data {.pos = 0, .size = 0, .active = 0};
                file.write(reinterpret_cast<const char *>(&data), sizeof(index_data));
                if (file.fail()) throw std::runtime_error("Unable to write to index file.");
            }

            else if (size % sizeof(index_data) not_eq 0)
            {
                throw std::runtime_error("The existing index file is incompatible with this version.");
            }
        }

    };

}
#pragma once

#include <string>
#include <fstream>
#include <vector>
#include <optional>

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

        auto get_last_ix() const -> std::optional<uint_t>
        {
            std::ifstream file(index_file_, std::ios::binary | std::ios::ate);

            // Check if index file is readable
            if (not file) throw std::runtime_error("Unable to open the index file.");

            file.seekg(0, std::ios::end);
            auto pos = file.tellg();

            if (pos % sizeof(index_data) not_eq 0) throw std::runtime_error("Incompatible version of index file.");
        
            uint_t last_ix = pos / sizeof(index_data) - 1;
            if (last_ix == 0) return {};

            return last_ix;
        }

        auto write(const uint_t i, const index_data &data) const 
        {
            std::fstream file(index_file_, std::ios::binary | std::ios::in | std::ios::out);

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

        auto read(const uint_t i) const -> std::optional<full_index_data>
        {
            auto max = get_last_ix();
            if (i > max) return {};

            std::ifstream file(index_file_, std::ios::binary);

            // Check if index file is readable
            if (not file) throw std::runtime_error("Unable to open the index file.");

            // Check if the file has the right size
            std::streampos pos = sizeof(index_data) * i;
            file.seekg(pos, std::ios::beg);

            full_index_data fdata;
            file.read(reinterpret_cast<char *>(&fdata.data), sizeof(fdata.data));
            if (file.fail()) throw std::runtime_error("Unable to read from index file.");

            if (not fdata.data.active) return {};

            fdata.ix = i;
            return fdata;
        }


        auto read_range(uint_t i, uint_t j) const -> std::vector<full_index_data>
        {
            auto max = get_last_ix();
            j = std::min(max, j);

            std::ifstream file(index_file_, std::ios::binary);

            // Check if index file is readable
            if (not file) throw std::runtime_error("Unable to open the index file.");

            // Check if the file has the right size
            std::streampos pos = sizeof(index_data) * i;
            file.seekg(pos, std::ios::beg);

            // std::streampos pos_end = sizeof(index_data) * (j + 1);
            // std::streampos to_read = pos_end - pos;

            std::vector<full_index_data> data;
            index_data d;
            while (i <= j)
            {
                file.read(reinterpret_cast<char *>(&d), sizeof(d));
                if (d.active) data.emplace_back(i, d);
                ++i;
            }
            
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
            index_data data;
            if (auto size = file.tellp(); size == 0)
            {
                file.write(reinterpret_cast<const char *>(&data), sizeof(data));
                if (file.fail()) throw std::runtime_error("Unable to write to index file.");
            }

            else if (size % sizeof(data) not_eq 0)
            {
                throw std::runtime_error("The existing index file is incompatible with this version.");
            }
        }
    };

}
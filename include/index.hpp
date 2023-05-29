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
        }

        auto create(const uint_t i, const uint_t s)
        {

        }

    private:

        const std::string index_file_;

        static auto init_identity(const std::string &index_file)
        {
            std::ofstream file(index_file, std::ios::binary | std::ios::app);

            // Check if index file is writable
            if (not file) std::runtime_error("Unable to open the index file.");

            // Check if the file has the right size
            file.seekp(0, std::ios::end);
            
            // Check if .id file does not exists
            if (auto size = file.tellp(); size == 0)
            {

                file.write(reinterpret_cast<const char *>(&ix), sizeof(uint_t));
                if (file.fail()) std::runtime_error("Unable to write to init identity file.");
            }

            // Check if the .id file is compatible
            else if (size % ix not_eq sizeof(uint_t))
            {
                std::runtime_error("The existing identity file is incompatible with this version.");
            }
        }

    };

}
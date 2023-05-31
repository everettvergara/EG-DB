#pragma once

#include <filesystem>
#include <string>
#include <fstream>

#include "common.hpp"

namespace eg
{
    class identity
    {
    public:
        identity(const std::string &table_name)
            :   identity_file_(table_name + ".id"),
                next_id_(identity::load_identity(identity_file_))
        {
        }


        auto get_next_id() -> uint_t 
        {
            std::ofstream file(identity_file_, std::ios::binary);
            if (not file) throw std::runtime_error("Could not open the identity file.");

            ++next_id_;

            file.write(reinterpret_cast<const char *>(&next_id_), sizeof(uint_t));
                if (file.fail()) throw std::runtime_error("Unable to write to init identity file.");

            return next_id_;
        }

    private:

        const std::string identity_file_;
        uint_t next_id_;

        static auto init_identity(const std::string &identity_file)
        {
            std::ofstream file(identity_file, std::ios::binary | std::ios::app);

            // Check if index file is writable
            if (not file) throw std::runtime_error("Unable to open the identity file.");

            // Check if the file has the right size
            file.seekp(0, std::ios::end);
            
            // Check if .id file does not exists
            if (auto size = file.tellp(); size == 0)
            {
                uint_t init = 0;
                file.write(reinterpret_cast<const char *>(&init), sizeof(uint_t));
                if (file.fail()) throw std::runtime_error("Unable to write to init identity file.");
            }

            // Check if the .id file is compatible
            else if (size not_eq sizeof(uint_t))
            {
                throw std::runtime_error("The existing identity file is incompatible with this version.");
            }
        }

        static auto load_identity(const std::string &identity_file) -> uint_t
        {
            // Initialize the .id file if it does not exist
            identity::init_identity(identity_file);

            // Try to open the file to get the last id
            std::ifstream file(identity_file, std::ios::binary);
            if (not file) std::runtime_error("Could not open the identity file.");

            // Get the next id
            uint_t next_id;
            file.read(reinterpret_cast<char *>(&next_id), sizeof(uint_t));
            if (file.fail()) std::runtime_error("Unable to read from init identity file.");

            return next_id;
        }
                
    };
}


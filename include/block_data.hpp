#pragma once

#include <cstdint> 
#include <iostream>
#include <fstream>
#include <stdexcept>

#include "common.hpp"
#include "raw.hpp"

namespace eg
{

    template <typename T>
    auto save_block_data(const std::string &filename, const T &data, const uint_t i)
    {
        // Assumes file already exists, even if it's 0 bytes.
        // Otherwise it will fail.
        std::fstream file(filename, std::ios::binary | std::ios::in | std::ios::out);
        if (not file) throw std::runtime_error("Could not open the block file.");

        // Check if the file has the right size
        file.seekp(0, std::ios::end);
        if (file.fail()) throw std::runtime_error("Could not get the file block size.");

        // Check if version is compatible
        auto file_size = file.tellp();
        if (file_size % sizeof(T) not_eq 0) throw std::runtime_error("Version of block size is incompatible.");
        
        file.seekp(i * sizeof(T), std::ios::end);
        file.write(reinterpret_cast<const char *>(&data), sizeof(T));
        if (file.fail()) throw std::runtime_error("Could not write to the file block size.");
    }

    template <typename T>
    auto load_block_data(const std::string &filename, const uint_t i) -> T
    {
        T data;


        return data;

    }
}
#pragma once

#include <cstdint> 
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <type_traits>
#include <filesystem>

#include "common.hpp"
#include "raw.hpp"

namespace eg
{

    auto create_file_if_not_exists(const std::string &filename)
    {
        if (not std::filesystem::exists(filename)) 
            std::ofstream file(filename, std::ios::binary);
    }

    auto get_file_handler_for_write_block_data(const std::string &filename) -> std::fstream
    {

        create_file_if_not_exists(filename);

        std::fstream file(filename, std::ios::binary | std::ios::in | std::ios::out);
        if (not file) throw std::runtime_error("Could not open the block file.");

        return file;
    }

    template <typename T>
    auto write_block_data(std::fstream &file, const T &data, const uint64_t i)
    {
        static_assert(std::is_trivially_copyable_v<T>, "Datastruct too complex.");

        // Check if the file has the right size
        file.seekp(0, std::ios::end);
        if (file.fail()) throw std::runtime_error("Could not get the file block size.");

        // Check if version is compatible
        auto file_size = file.tellp();
        if (file_size % sizeof(T) not_eq 0) throw std::runtime_error("Version of block size is incompatible.");
        
        file.seekp(i * sizeof(T), std::ios::beg);
        file.write(reinterpret_cast<const char *>(&data), sizeof(T));
        if (file.fail()) throw std::runtime_error("Could not write to the file block.");
    }

    template <typename T>
    auto write_block_data(const std::string &filename, const T &data, const uint64_t i)
    {
        static_assert(std::is_trivially_copyable_v<T>, "Datastruct too complex.");
        
        std::fstream file = get_file_handler_for_write_block_data(filename);
        write_block_data(file);
    }

    template <typename T>
    auto read_block_data(const std::string &filename, const uint64_t i) -> T
    {
        static_assert(std::is_trivially_copyable_v<T>, "Datastruct too complex.");

        // Assumes file already exists, even if it's 0 bytes.
        // Otherwise it will fail.
        std::ifstream file(filename, std::ios::binary);
        if (not file) throw std::runtime_error("Could not open the block file.");

        // Check if the file has the right size
        file.seekg(0, std::ios::end);
        if (file.fail()) throw std::runtime_error("Could not get the file block size.");

        // Check if version is compatible
        auto file_size = file.tellg();
        if (file_size % sizeof(T) not_eq 0) throw std::runtime_error("Version of block size is incompatible.");
        
        // Check if i is within the range
        auto last_i = file_size / sizeof(T) - 1;
        if (i > last_i) throw std::runtime_error("Invalid index to load.");

        T data;
        file.seekg(i * sizeof(T), std::ios::beg);
        file.read(reinterpret_cast<char *>(&data), sizeof(T));
        if (file.fail()) throw std::runtime_error("Could not read to the file block.");

        return data;
    }
}
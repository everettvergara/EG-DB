#pragma once

#include <cstdint> 
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <type_traits>
#include <filesystem>
#include <optional>

#include "common.hpp"
#include "raw.hpp"

namespace eg
{
    auto create_file_if_not_exists(const std::string &filename)
    {
        if (not std::filesystem::exists(filename)) 
            std::ofstream file(filename, std::ios::binary);
    }

    auto get_file_handler_for_read_write_data(const std::string &filename) -> std::fstream
    {
        create_file_if_not_exists(filename);

        std::fstream file(filename, std::ios::binary | std::ios::in | std::ios::out);
        if (not file) throw std::runtime_error("Could not open the block file.");

        return file;
    }

    auto get_file_size(std::fstream &file) -> std::streampos
    {
        file.seekg(0, std::ios::end);
        if (file.fail()) throw std::runtime_error("Could not set the file stream loc.");

        auto file_size = file.tellg();
        if (file.fail()) throw std::runtime_error("Could not get the file stream loc.");

        return file_size;
    }

    template <typename T>
    auto write_data(std::fstream &file, const std::streampos pos, const T *data)
    {
        file.seekp(pos, std::ios::beg);
        file.write(reinterpret_cast<const char *>(data), sizeof(T));
        if (file.fail()) throw std::runtime_error("Could not write to the file.");     
    }

    template <typename T>
    auto read_data(std::fstream &file, const std::streampos pos) -> T
    {
        T data;
        file.seekg(pos, std::ios::beg);
        file.read(reinterpret_cast<char *>(&data), sizeof(T));
        if (file.fail()) throw std::runtime_error("Could not read from the file block.");

        return data;        
    }

    template <typename T>
    auto read_data(std::fstream &file, const std::streampos pos, T *data) -> T *
    {
        file.seekg(pos, std::ios::beg);
        file.read(reinterpret_cast<char *>(data), sizeof(T));
        if (file.fail()) throw std::runtime_error("Could not read from the file block.");

        return data;        
    }

    template <typename T>
    auto write_block_data(std::fstream &file, const uint64_t i, const T *data)
    {
        static_assert(std::is_trivially_copyable_v<T>, "Datastruct too complex.");
        write_data<T>(file, i * sizeof(T), data);
    }

    template <typename T>
    auto get_last_ix_of_block_data(std::fstream &file) -> std::optional<uint64_t>
    {
        auto file_size = get_file_size(file);
        if (file_size == 0) return {};

        if (file_size % sizeof(T) not_eq 0) throw std::runtime_error("Version of block size is incompatible.");
        auto last_ix = file_size / sizeof(T) - 1;
        
        return last_ix;
    }

    template <typename T>
    auto validate_block_data_ix(std::fstream &file, const uint64_t i) -> std::optional<uint64_t>
    {
        auto last_ix = get_last_ix_of_block_data<T>(file);
        if (not last_ix.has_value()) return {};
        if (i > last_ix.value()) throw std::runtime_error("Invalid index to load.");
    
        return last_ix;
    }

    template <typename T>
    auto read_block_data(std::fstream &file, const uint64_t i) -> std::optional<T>
    {
        if (not validate_block_data_ix<T>(file, i).has_value()) return {};
        return read_data<T>(file, i * sizeof(T));
    }

    template <typename T>
    auto read_block_data(std::fstream &file, const uint64_t i, T *data) -> T *
    {
        if (not validate_block_data_ix<T>(file, i).has_value()) return nullptr;
        return read_data<T>(file, i * sizeof(T), data);
    }


}
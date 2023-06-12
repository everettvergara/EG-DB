#pragma once

#include <string>
#include <type_traits>
#include <cstring>
#include <vector>
#include <stdexcept>

#include "common.hpp"
#include "column_attr.hpp"

namespace eg
{
    class column
    {
    private: 
        
        column_attr             attr_;
        std::vector<uint8_t>    data_;

    public:

        column(const column_attr attr)
            :   attr_(attr), 
                data_(sizeof(uint_t) + (attr_.is_varsize ? 0 : attr_.max_size), '\0')
        {
            uint_t sizeof_column = attr_.is_varsize ? 0 : attr_.max_size;
            std::memcpy(data_.data(), &sizeof_column, sizeof(uint_t));
        }

        column(const column &rhs)
            :   attr_(rhs.attr_),
                data_(rhs.data_)
        {
        }

        column(column &&rhs)
            :   attr_(rhs.attr_)
        {
            data_ = std::move(rhs.data_);
        }

        auto get_name() const -> const char *
        {
            return attr_.name;
        }

        auto get_raw_size() const -> uint_t
        {
            return data_.size();
        }

        auto get_ptr_to_raw_data() const -> const uint8_t *
        {
            return data_.data();
        }

        auto get_data_size() const -> uint_t
        {
            return data_.size() - sizeof(uint_t);
        }        

        auto get_ptr_to_data() const -> const uint8_t *
        {
            return data_.data() + sizeof(uint_t);
        }

        template <typename T>
        auto get_ptr_to_data_by_type() -> T *
        {
            return reinterpret_cast<T *>(data_.data() + sizeof(uint_t));
        }

        template <typename T>
        auto get_ptr_to_data_by_type() const -> const T *
        {
            return reinterpret_cast<T *>(data_.data() + sizeof(uint_t));
        }

        auto operator =(const column &rhs) -> column &
        {
            // Check if assigning to self
            if (this == &rhs) return *this;

            // Validation
            if (not attr_.is_varsize and attr_.max_size not_eq rhs.attr_.max_size)
                std::runtime_error("Incompatible column assignment");

            else if (attr_.is_varsize and attr_.max_size < rhs.get_raw_size())
                std::runtime_error("Size of column is less than the RHS");

            // Copy data from rhs
            data_ = rhs.get_raw_data();
        }

        auto operator =(column &&rhs) -> column &
        {
            // Check if assigning to self
            if (this == &rhs) return *this;

            // Validation
            if (not attr_.is_varsize and attr_.max_size not_eq rhs.attr_.max_size)
                std::runtime_error("Incompatible column assignment.");

            else if (attr_.is_varsize and attr_.max_size < rhs.get_raw_size())
                std::runtime_error("Size of column is less than the RHS.");

            // Move data from rhs
            data_ = std::move(rhs.get_raw_data());
        }

        template <typename T, 
                    typename std::enable_if_t<
                                not std::is_same_v<T, column>
                                and not std::is_same_v<T, std::string>
                                and not std::is_same_v<T, std::wstring>, int> = 0>
        
        auto operator =(const T &rhs) -> column &
        {
            // Compile validation
            static_assert(std::is_trivially_copyable_v<T>, "Data type not supported. Too complex.");

            // Runtime validation
            constexpr uint_t size_of_t = sizeof(T);

            if (not attr_.is_varsize and attr_.max_size not_eq size_of_t)
                std::runtime_error("Incompatible column assignment. Not the same size.");

            else if (attr_.is_varsize and attr_.max_size < size_of_t)
                std::runtime_error("Incompatible column assignment. Size of T exceeds column size.");

            // Resize if applicable
            if (attr_.is_varsize)
                data_.resize(sizeof(uint_t) + size_of_t);

            // Copy 
            std::memcpy(data_.data(), &size_of_t, size_of_t);
            std::memcpy(data_.data() + sizeof(uint_t), &rhs, size_of_t);

            return *this;
        }

        template <typename T, 
                    typename std::enable_if_t<
                                std::is_same_v<T, std::string>
                                or std::is_same_v<T, std::wstring>, int> = 0>
        
        auto operator =(const T &rhs) -> column &
        {
            uint_t size_of_t = rhs.size();

            if (attr_.max_size < size_of_t)
                std::runtime_error("Incompatible column assignment. Size of T exceeds column size.");

            // Resize if applicable
            if (attr_.is_varsize)
                data_.resize(sizeof(uint_t) + size_of_t);

            // Copy 
            std::memcpy(data_.data(), &size_of_t, size_of_t);
            std::memcpy(data_.data() + sizeof(uint_t), rhs.data(), size_of_t);

            return *this;
        }        

        auto get_raw_data() const -> const std::vector<uint8_t> &
        {
            return data_;
        }


    };    
}


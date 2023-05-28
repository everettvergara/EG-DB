#pragma once

#include <cstdint>
#include <vector>
#include <cstring>
#include <fstream>

namespace eg
{
    class raw
    {
    public:

        auto operator <<(const std::vector<uint8_t> &t) -> raw &
        {
            if (t.empty()) 
                return *this;

            auto start = data_.size();
            data_.resize(start + t.size());
            std::memcpy(data_.data() + start, t.data(), t.size());

            return *this;
        }

        auto operator <<(const raw &t) -> raw &
        {
            if (t.data_.empty()) 
                return *this;

            auto start = data_.size();
            data_.resize(start + t.data_.size());
            std::memcpy(data_.data() + start, t.data_.data(), t.data_.size());

            return *this;
        }

        friend auto operator <<(std::ofstream &fs, const raw &t) -> std::ofstream &
        {
            fs.write(reinterpret_cast<const char *>(t.data_.data()), t.data_.size()); 

            return fs;
        }

    private:
        std::vector<uint8_t> data_;

    };
}
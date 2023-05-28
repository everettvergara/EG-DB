#pragma once

#include <initializer_list>
#include <cstdint>
#include <vector>
#include <unordered_map>
#include <optional>
#include "common.hpp"
#include "column.hpp"
#include "raw.hpp"

namespace eg
{
    class row
    {
    public:

        row(std::initializer_list<column> columns)
            :   id_(column_attr {   .name = "id", 
                                    .max_size = sizeof(uint_t), 
                                    .is_varsize = false}),
                columns_(columns)
        {
            for (uint_t i = 0; auto c : columns_)
                cardinality_.try_emplace(c.get_name(), i++); 
        }

        auto get_cardinality(const char * name) -> std::optional<uint_t>
        {
            if (auto f = cardinality_.find(name); f not_eq cardinality_.end())
                return f->second;
            return {};
        }

        auto get_column(const char *name) -> std::optional<column *>
        {
            auto ix = get_cardinality(name);
            if (not ix.value()) 
                return {};
            
            return &columns_.at(ix.value());
        }

        auto get_raw() -> raw
        {
            raw raw_data;

            for (auto &c : columns_)
                raw_data << c.get_raw_data();
            
            return raw_data;
        }

    protected:


    private:
        
        std::vector<column> columns_;
        std::unordered_map<const char *, uint_t> cardinality_;

    };
}
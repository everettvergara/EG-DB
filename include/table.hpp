#pragma once

#include <string>
#include <unordered_map>

#include "column_attr.hpp"
#include "column.hpp"
// #include "row.hpp"
// #include "page_ix.hpp"

namespace eg
{
    class table
    {
    public:

        table(const std::string &table_name, std::vector<column> &&columns)
            :   table_name_(table_name), 
                page_ix_name_(table_name_ + ".ix"),
                columns_(std::move(columns))
        {
            for (uint_t i = 0; auto c : columns_)
                column_cardinality_.try_emplace(c.get_name(), i++); 
        }

    private:

        std::string table_name_;
        std::string page_ix_name_;
        
        std::vector<column> columns_;
        std::unordered_map<const char *, uint_t> column_cardinality_;
    };
}


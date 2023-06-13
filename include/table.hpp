#pragma once

#include <string>
#include <unordered_map>

#include "column_attr.hpp"
#include "column.hpp"
#include "file_helper.hpp"
#include "page_ix.hpp"
// #include "row.hpp"
// #include "page_ix.hpp"

namespace eg
{
    using PAGE_IX_TYPE              = uint8_t;
    constexpr uint64_t PAGE_IX_SIZE = 256;

    class table
    {
    public:

        table(const std::string &table_name, std::vector<column> &&columns)
            :   table_name_(table_name), 
                page_ix_name_(table_name_ + ".ix"),
                columns_(std::move(columns)),
                last_page_ix_no_(this->get_last_page_ix_no())
        {
            for (uint_t i = 0; auto c : columns_)
                column_cardinality_.try_emplace(c.get_name(), i++); 
        }

        auto get_last_page_ix_no() -> uint64_t
        {
            auto file = get_file_handler_for_read_write_data(page_ix_name_);

            auto last = get_last_ix_of_block_data<page_ix_data<PAGE_IX_TYPE, PAGE_IX_SIZE>>(file);
            return not last.has_value() ? 0 : last.value();
        }

    private:

        std::string table_name_;
        std::string page_ix_name_;
        
        std::vector<column> columns_;

        // write a column cardinality initializer for constructor
        std::unordered_map<const char *, uint_t> column_cardinality_;

        uint64_t last_page_ix_no_;
    };
}


#pragma once

#include <string>

namespace eg
{
    class index
    {
    public:

        index(const std::string &table_name)
            : index_file_(table_name + ".ix")
        {
        }

    private:

        const std::string index_file_;

    };

}
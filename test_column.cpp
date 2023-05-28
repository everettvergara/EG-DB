#include <iostream>
#include <string>

#include "column_attr.hpp"
#include "column.hpp"

auto main(int, char *[]) -> int
{
    // Constructor test
    eg::column_attr id_attr{.name = "ID", .max_size = sizeof(eg::uint_t), .is_varsize = false};
    eg::column_attr name_attr{.name = "Name", .max_size = 100, .is_varsize = true};

    eg::column id(id_attr);
    eg::column name(name_attr);

    std::cout << "id.get_raw_size(): " << id.get_raw_size() << std::endl;
    std::cout << "id.get_data_size(): " << id.get_data_size() << std::endl;

    // int assignment
    id = 100;
    std::cout << "id.get_data(): " << *id.get_ptr_to_data_by_type<int>() << std::endl;

    // std::string assignment
    std::cout << "name.get_raw_size() before assignment: " << name.get_raw_size() << std::endl;
    std::cout << "name.get_data_size() before assignment: " << name.get_data_size() << std::endl;
    std::string test = {"Everett"};
    name = test;
    std::cout << "name.get_raw_size() after assignment: " << name.get_raw_size() << std::endl;
    std::cout << "name.get_data_size() after assignment: " << name.get_data_size() << std::endl;
    std::cout.write(name.get_ptr_to_data_by_type<const char>(), name.get_data_size());
    std::cout << std::endl;

    return 0;
}

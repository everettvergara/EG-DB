#include <iostream>
#include <vector>
#include "page.hpp"

// using namespace eg;
auto main(int, char *[]) -> int
{
    using page_ix_data = eg::page_ix_data<uint8_t, 30>;
    std::vector<page_ix_data, eg::no_init<page_ix_data>> data(1);


    std::cout << static_cast<int>(data.at(0).status[0]) << std::endl;

    return 0;
}
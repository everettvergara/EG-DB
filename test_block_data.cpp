
#include <iostream>

#include "block_data.hpp"

using namespace eg;

struct data_sample
{
    uint_t x;
    uint_t y;

};

auto main(int, char *[]) -> int
{

    data_sample data {.x = 100, .y = 100};

    // save_block_data<data_sample>("block_data_sample.data", data, 10);


    return 0;
}
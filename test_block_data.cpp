
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

    {
        data_sample data {.x = 100, .y = 100};
        save_block_data<data_sample>("block_data_sample.data", data, 1);

        auto data1 = load_block_data<data_sample>("block_data_sample.data", 0);
        std::cout << "data1." << data1.x << std::endl;    

        auto data2 = load_block_data<data_sample>("block_data_sample.data", 1);
        std::cout << "data2." << data2.x << std::endl;    

        data_sample data3 {.x = 200, .y = 200};
        save_block_data<data_sample>("block_data_sample.data", data3, 2);
        auto data33 = load_block_data<data_sample>("block_data_sample.data", 2);
        std::cout << "data33." << data33.x << std::endl;
        auto data44 = load_block_data<data_sample>("block_data_sample.data", 3);
        std::cout << "data44." << data44.x << std::endl;

    }





    return 0;
}
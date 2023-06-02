
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

        // auto data3 = load_block_data<data_sample>("block_data_sample.data", 2);
        // std::cout << "data3." << data3.x << std::endl;    
    }

    // auto data3 = load_block_data<data_sample>("block_data_sample.data", 10);
    // std::cout << "data3." << data3.x << std::endl;

    // auto data4 = load_block_data<data_sample>("block_data_sample.data", 11);
    // std::cout << "data4." << data4.x << std::endl;



    return 0;
}
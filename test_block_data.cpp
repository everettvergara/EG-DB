
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

    auto file = get_file_handler_for_write_block_data("block_data_sample.data");
    

    // {
    //     data_sample data {.x = 100, .y = 100};
    //     write_block_data<data_sample.data", data, 1);

    //     // Should show 0, because it is skipped 
        
    //     auto data1 = read_block_data<data_sample>("block_data_sample.data", 0);
    //     std::cout << "data1." << data1.x << std::endl;    

        
    //     // Should show 100 
    //     auto data2 = read_block_data<data_sample>("block_data_sample.data", 1);
    //     std::cout << "data2." << data2.x << std::endl;    


    //     // Should show 200 
    //     data_sample data3 {.x = 200, .y = 200};
    //     write_block_data<data_sample>("block_data_sample.data", data3, 2);
    //     auto data33 = read_block_data<data_sample>("block_data_sample.data", 2);
    //     std::cout << "data33." << data33.x << std::endl;


    //     // Should show 100 
    //     auto data22 = read_block_data<data_sample>("block_data_sample.data", 1);
    //     std::cout << "data22." << data22.x << std::endl;    


    //     // Should throw an error 
    //     auto data44 = read_block_data<data_sample>("block_data_sample.data", 3);
    //     std::cout << "data44." << data44.x << std::endl;

    // }





    return 0;
}
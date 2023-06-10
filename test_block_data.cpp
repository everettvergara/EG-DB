
#include <iostream>
#include <vector>
#include <ranges>

#include "block_data.hpp"

using namespace eg;

struct data_sample
{
    uint_t x;
    uint_t y;

};

auto main(int, char *[]) -> int
{
    // Initialization
    auto N = 10;
    std::vector<data_sample> vec(N);
    for (uint_t i = 0; auto &v : vec)
    {
        v.x = i + 1;
        v.y = v.x * v.x;
    }

    auto file = get_file_handler_for_write_block_data("block_data_sample.data");

    std::cout << "File size: " << get_file_size(file) << std::endl;
    
    // Skip the first 3 and write
    std::cout << "data_sample_size: " << sizeof(data_sample) << std::endl;
    std::cout << "Writing 10 blocks of data_sample: " << get_file_size(file) << std::endl;
    auto skip = 3;
    std::cout << "skipped records: " << skip << std::endl;
    
    auto selection = vec | std::views::drop(skip);
    for (auto i = skip; auto s : selection)
    {
        std::cout << "writing: " << i << std::endl;
        write_block_data<data_sample>(file, s, i);
        ++i;
    }

    std::cout << "Expecting size after write: " << sizeof(data_sample) * N << std::endl;
    std::cout << "File size after write: " << get_file_size(file) << std::endl;

    for (auto i = skip; auto s : selection)
    {
    }

    return 0;
}



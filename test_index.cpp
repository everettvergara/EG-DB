#include <iostream>
#include <cstdlib>
#include "identity.hpp"
#include "index_data.hpp"
#include "index.hpp"


auto main(int, char *[]) -> int
{
    eg::identity id("tb_sys_mf_user");
    eg::index ix("tb_sys_mf_user");

    auto i = ix.get_last_ix(); // id.get_next_id();

    std::cout << "i: " << i << std::endl;
    auto data1 = ix.read(i);
    auto data2 = ix.read(i);
    auto data3 = ix.read(i);

    std::cout << "data1.pos: " << data1.pos << " data1.size: " << data1.size << " data1.active: " << data1.active << std::endl; 
    std::cout << "data2.pos: " << data2.pos << " data2.size: " << data2.size << " data2.active: " << data2.active << std::endl; 
    std::cout << "data3.pos: " << data3.pos << " data3.size: " << data3.size << " data3.active: " << data3.active << std::endl; 
//    auto data3 = ix.read(i - 1);
    
// ix.write(i, eg::index_data{.pos = static_cast<eg::uint_t>(i + rand() % i), .size = static_cast<eg::uint_t>(i + rand() % i), .active = true});
    
    // auto vec_data = ix.read_range(0, i);
    // for (auto j = 0; const auto &v : vec_data)
    // {
    //     std::cout << j++ << ": data.pos: " << v.pos << " data.size: " << v.size << " data.active: " << v.active << std::endl; 
    // }

    // auto data2 = ix.read(i);

    return 0;
}
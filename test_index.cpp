#include <iostream>

#include "identity.hpp"
#include "index_data.hpp"
#include "index.hpp"


auto main(int, char *[]) -> int
{
    eg::identity id("tb_sys_mf_user");
    eg::index ix("tb_sys_mf_user");

    auto i = id.get_next_id();

    std::cout << "i: " << i << std::endl;
    ix.write(i, 10, 100, true);

    auto data = ix.read(i);

    std::cout << "data.pos: " << data.pos << " data.size: " << data.size << " data.active: " << data.active << std::endl; 

    // auto vec_data = ix.read_range(0, i);
    // for (auto j = 0; const auto &v : vec_data)
    // {
    //     std::cout << j++ << ": data.pos: " << v.pos << " data.size: " << v.size << " data.active: " << v.active << std::endl; 
    // }

    auto data2 = ix.read(i - 1);
    std::cout << "data2.pos: " << data2.pos << " data2.size: " << data2.size << " data2.active: " << data2.active << std::endl; 

    return 0;
}
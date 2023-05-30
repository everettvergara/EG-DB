#include <iostream>
#include <cstdlib>
#include "identity.hpp"
#include "index_data.hpp"
#include "index.hpp"


auto main(int, char *[]) -> int
{
    eg::identity id("tb_sys_mf_user");
    eg::index ix("tb_sys_mf_user");

    auto i = ix.get_last_ix();

    
    auto vec_data = ix.read_range(0, i);
    for (auto j = 0; const auto &v : vec_data)
    {
        std::cout << j++ << ": data.pos: " << v.pos << " data.size: " << v.size << " data.active: " << v.active << std::endl; 
    }

    return 0;
}
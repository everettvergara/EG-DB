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

    // eg::index_data d(0, 0, 0);
    // ix.write(7, d);
    
    auto vec_data = ix.read_range(0, i+2);
    for (const auto &v : vec_data)
    {
        std::cout << v.ix << ": data.pos: " << v.data.pos << " data.size: " << v.data.size << " data.active: " << v.data.active << std::endl; 
    }

    return 0;
}

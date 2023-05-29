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


    return 0;
}
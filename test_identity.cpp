#include <iostream>
#include <unordered_set>
#include "identity.hpp"

auto main(int, char *[]) -> int
{
    eg::identity tb_sys_mf_user_id("tb_sys_mf_user");

    auto next = tb_sys_mf_user_id.get_next_id();
    auto curr = tb_sys_mf_user_id.get_curr_id();

    std::cout << next << " " << curr << std::endl;



    return 0;
}



#include <iostream>
#include "block_data.hpp"

using namespace eg;


struct derived_block // : public block_data
{
    uint_t x;
    uint_t y;

    auto get_size() -> size_t // override
    {
        return sizeof(derived_block);
    }

    auto temp()
    {
        auto s = get_size();
        std::cout << " size of derived" << s << std::endl;


    }



};


auto main(int, char *[]) -> int
{

    derived_block db;

    db.temp();

    return 0;
}
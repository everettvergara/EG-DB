#include <iostream>
#include "common.hpp"
#include "block_data.hpp"
#include "page_ix.hpp"

auto main(int, char *[]) -> int
{

    auto file = eg::get_file_handler_for_read_write_data("page_ix.data");
    
    eg::page_ix<uint8_t, 10> pix(file, 0, eg::page_ix_construct_option::INIT);

    constexpr auto s = sizeof(eg::page_ix_data<uint8_t, 10>);
    std::cout << "expected size: " << s << std::endl;
    std::cout << "actual size: " << eg::get_file_size(file) << std::endl;

    auto last_id = pix.generate_next_id(file, 0, 1000);

    auto last_id_gen = pix.get_last_gen_id();
    
    if (last_id_gen.has_value())
    {
        std::cout << "last id gen: " << static_cast<int>(last_id) << std::endl;
        std::cout << "last id gen: " << static_cast<int>(last_id_gen.value()) << std::endl;
    }
    else 
        std::cout << "no ID gen!" << std::endl;
    return 0;
}

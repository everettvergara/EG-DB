#include <iostream>
#include "common.hpp"
#include "block_data.hpp"
#include "page_ix.hpp"

auto main(int, char *[]) -> int
{

    std::string filename {"page_ix.data"};

    auto file = eg::get_file_handler_for_read_write_data(filename);
    
    auto curr_file_size = eg::get_file_size(file);

    auto option = curr_file_size == 0 ? eg::page_ix_construct_option::INIT : eg::page_ix_construct_option::LOAD;
    
    std::cout << "init: " << static_cast<int>(option) << std::endl; 
    eg::page_ix<uint8_t, 255> pix(file, 0, option);

    constexpr auto s = sizeof(eg::page_ix_data<uint8_t, 255>);
    std::cout << "expected size: " << s << std::endl;
    std::cout << "actual size: " << eg::get_file_size(file) << std::endl;


    auto heap_pos = pix.get_last_gen_id().has_value() ? (pix.get_last_gen_id().value() + 1) * 1000 : 0; 

    auto last_id = pix.generate_next_id(file, 0, heap_pos);

    auto last_id_gen = pix.get_last_gen_id();
    
    if (last_id_gen.has_value())
    {
        std::cout << "last id gen: " << static_cast<int>(last_id) << std::endl;
        std::cout << "last id gen: " << static_cast<int>(last_id_gen.value()) << std::endl;
    }
    else 
    {
        std::cout << "no ID gen!" << std::endl;
    }

    // ---------------------------

    // Delete id: 5
    pix.delete_id(file, 0, {7, 8, 9});

    std::cout << "Retrieve all active in this page_ix:" << std::endl;
    for (auto [id, heap_pos] : pix.retrieve_heap_pos_all_active())
    {
        std::cout << static_cast<int>(id) << ": " << heap_pos << std::endl; 
    }


    return 0;
}

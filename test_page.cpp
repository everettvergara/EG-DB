#include <iostream>
#include "page.hpp"

auto main(int, char *[]) -> int
{

    auto file = eg::get_file_handler_for_read_write_data("read_write_sample.ix");
//     eg::page_ix<uint8_t, 255> pix(file, 0, eg::page_ix_construct_option::INIT);
    eg::page_ix<uint8_t, 255> pix(file, 0, eg::page_ix_construct_option::LOAD);


    return 0;
}

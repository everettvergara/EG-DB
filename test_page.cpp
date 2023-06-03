#include <iostream>
#include "page.hpp"

// using namespace eg;
auto main(int, char *[]) -> int
{

    eg::page<256> p0;
    eg::page<256> p1;

    p0.init();
    p1.init();

    p0.save("test_page.ix", 0);

    p1.save("test_page.ix", 1);




    return 0;
}
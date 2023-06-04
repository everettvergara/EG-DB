#include <iostream>
#include "page.hpp"

// using namespace eg;
auto main(int, char *[]) -> int
{

    eg::page<20> p0;
    eg::page<20> p1;

    p0.init();
    p1.init();

    p0.save("test_page.ix", 0);

    p1.save("test_page.ix", 1);

    p1.debug();

    return 0;
}
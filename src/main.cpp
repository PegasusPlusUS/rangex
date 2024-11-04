#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
//#include "doctest.h"
#include <iostream>

#include "rangex_lib.h"
using namespace ns_rangex;

int main() {

    for(auto iv : rangex<int, true>(1, 70, 3, true)) {
        std::cout << "i:" << iv.first << " v:" << iv.second << std::endl;
    }

    return 0;
}
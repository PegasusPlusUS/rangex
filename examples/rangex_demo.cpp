#define COMPILER_HAS_STD_FLOAT

#include "rangex_lib.h"
using namespace ns_rangex;

#include <iostream>
#include <cstdint>
#include <algorithm> // for_each()
#include <numeric> // accumulate

int main() {
    printCompilerInfo();
    std::cout << "Compiler " << (checkCompilerHasStdFloat() ? "HAS" : "has NO") << " stdfloat support." << std::endl;
    std::cout << "COMPILER_HAS_STD_FLOAT " 
#ifdef COMPILER_HAS_STD_FLOAT
    << " "
#else
    << "NOT "
#endif
    << "defined." << std::endl << std::endl;

    std::uint16_t sum = 0;
    rangex<std::uint8_t> r = rangex<std::uint8_t>(1, 100, true);
    for (auto i : r) {
        sum += i;
    }
    std::printf("Calculate sum(1...100) by several means:\n");
    std::printf("Using for(auto i:r): %u\n", sum);

    sum = 0;
    std::for_each(r.begin(), r.end(), [&sum](auto i) { sum += i; });
    std::cout << "Using std::for_each() lamda in <algorithm>: " << sum << std::endl;

    sum = std::accumulate(r.begin(), r.end(), 0);
    std::cout << "Using std::accumulate in <numeric>: " << sum << std::endl;    
}

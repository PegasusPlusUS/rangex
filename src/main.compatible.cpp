#include "test_framework.h"

#include <iostream>
#include <cstdint>
#include <type_traits>
#include <cassert>

#define has_no_std_float
#include "rangex_lib.h"
using namespace ns_rangex;

template <typename T>
void verify_for_loop_range_detail(T expect[], size_t expect_len, rangex<T, false, true> r) {
    T sum = 0;
    size_t index = 0;
    for(auto v : r) {
        static_assert(check_eq_typeof<decltype(v), T>());
        sum += v;
        CHECK(index < expect_len);
        CHECK_EQ(v, expect[index++]);
    }
    CHECK(index == expect_len);
}

TEST_CASE_EX(rangex_test, inclusive_range_downward_of_float_default_detail) {
    // float inclusive step -1
    // get 5...1 by -1
    using element_type_t = std::float32_t;
    #define element_type_bits 32
    element_type_t expect[] = {scf<element_type_bits>(5.0f), scf<element_type_bits>(4.0f), scf<element_type_bits>(3.0f), scf<element_type_bits>(2.0f), scf<element_type_bits>(1.0f)};
    verify_for_loop_range_detail<element_type_t>(expect, sizeof(expect)/sizeof(expect[0]), rangex<element_type_t, false, true>(scf<element_type_bits>(5.0f), scf<element_type_bits>(1.0f), true, scf<element_type_bits>(-1.0f)));
}

// Failed on Ubuntu-latest on GitHub
// [  FAILED  ] 3 tests, listed below:
// [  FAILED  ] rangex_test.inclusive_range_downward_of_float_default
// [  FAILED  ] rangex_test.inclusive_range_downward_of_float_explicitly
// [  FAILED  ] rangex_test.inclusive_indexed_range_downward_of_typename_float16_t

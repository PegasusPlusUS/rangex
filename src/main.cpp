#include "test_framework.h"

#include <iostream>
#include <cstdint>
#include <type_traits>
#include <cassert>

#include "rangex_lib.h"
using namespace ns_rangex;

// test_framework provides main()
TEST_CASE_EX(rangex_test, default_exclusive_range_of_int) {
    // default rangex<int, false> as normal exclusive loop at step 1
    // get 1...5

    using element_type_t = int;
    element_type_t sum = 0;
    element_type_t expect[] = {1, 2, 3, 4, 5};
    size_t index = 0;
    for(auto v : rangex(1, 6)) {
        static_assert(check_eq_typeof<decltype(v), element_type_t>());
        sum += v;
        CHECK(index < sizeof(expect)/sizeof(expect[0]));
        CHECK(v == expect[index++]);
        std::cout << v << " ";
    }
    CHECK_EQ(index, sizeof(expect)/sizeof(expect[0]));
    CHECK_EQ(sum, 1 + 2 + 3 + 4 + 5);
    std::cout << std::endl;
}

TEST_CASE_EX(rangex_test, inclusive_range_of_int) {
    // int inclusive step 1
    // get 1...5

    using element_type_t = int;
    element_type_t sum = 0;
    element_type_t expect[] = {1, 2, 3, 4, 5};
    size_t index = 0;
    for(auto v : rangex(1, 5, true)) {
        static_assert(check_eq_typeof<decltype(v), element_type_t>());
        sum += v;
        CHECK(index < 5);
        CHECK(v == expect[index++]);
        std::cout << v << " ";
    }
    CHECK(index == 5);
    CHECK(sum == 1 + 2 + 3 + 4 + 5);
    std::cout << std::endl;
}

TEST_CASE_EX(rangex_test, inclusive_range_downward_of_int_default) {
    // int inclusive step -1
    // get 5...1 by -1

    using element_type_t = int;
    element_type_t sum = 0;
    element_type_t expect[] = {5, 4, 3, 2, 1};
    size_t index = 0;
    for(auto v : rangex(5, 1, true, -1)) {
        static_assert(check_eq_typeof<decltype(v), element_type_t>());
        sum += v;
        CHECK(index < 5);
        CHECK(v == expect[index++]);
        std::cout << v << " ";
    }
    CHECK(index == 5);
    CHECK(sum == 5 + 4 + 3 + 2 + 1);
    std::cout << std::endl;
}

TEST_CASE_EX(rangex_test, inclusive_range_downward_of_int_explicitly_) {
    // int inclusive step -1
    // get 5...1 by -1

    using element_type_t = int;
    element_type_t sum = 0;
    element_type_t expect[] = {5, 4, 3, 2, 1};
    size_t index = 0;
    for(auto v : rangex<element_type_t>(5, 1, true, -1)) {
        static_assert(check_eq_typeof<decltype(v), element_type_t>());
        sum += v;
        CHECK(index < 5);
        CHECK(v == expect[index++]);
        std::cout << v << " ";
    }
    CHECK(index == 5);
    CHECK(sum == 5 + 4 + 3 + 2 + 1);
    std::cout << std::endl;
}

TEST_CASE_EX(rangex_test, inclusive_range_downward_of_uint8_t_explicitly_no_index_explicitly) {
    // int inclusive step -1
    // get 5...1 by -1

    using element_type_t = uint8_t;
    element_type_t sum = 0;
    element_type_t expect[] = {5, 4, 3, 2, 1};
    size_t index = 0;
    for(auto v : rangex<element_type_t, false>(5, 1, true, -1)) {
        static_assert(check_eq_typeof<decltype(v), element_type_t>());
        sum += v;
        CHECK(index < 5);
        CHECK(v == expect[index++]);
        std::cout << v << " ";
    }
    CHECK(index == 5);
    CHECK(sum == 5 + 4 + 3 + 2 + 1);
    std::cout << std::endl;
}

TEST_CASE_EX(rangex_test, inclusive_indexed_range_downward_of_typename_uint8_t) {
    // uint8_t inclusive step -1
    // get 5...1

    using element_type_t = uint8_t;
    element_type_t sum = 0;
    element_type_t expect[] = {5, 4, 3, 2, 1, 0};
    size_t index = 0;
    for(auto iv : rangex<element_type_t, true>(5, 0, true, -1)) {
        static_assert(check_eq_typeof<decltype(iv.first), size_t>());
        static_assert(check_eq_typeof<decltype(iv.second), element_type_t>());
        size_t auto_index = iv.first;
        element_type_t v = iv.second;
        sum += v;
        CHECK(auto_index == index);
        CHECK(index < sizeof(expect)/sizeof(expect[0]));
        CHECK(v == expect[index++]);
        std::cout << v << " ";
    }
    CHECK(index == sizeof(expect)/sizeof(expect[0]));
    CHECK(sum == 5 + 4 + 3 + 2 + 1 + 0);
    std::cout << std::endl;
}

// If end is not exactly on step, inclusive and exclusive get same result (verified on Swift)
TEST_CASE_EX(rangex_test, step_gt_1_and_how_to_though_end_not_just_stepped) {
    using element_type_t = int;
    element_type_t expect[] = {1, 4, 7};
    {
        element_type_t sum = 0;
        size_t index = 0;
        for(auto v : rangex(1, 9, true, 3)) {
            static_assert(check_eq_typeof<decltype(v), element_type_t>());
            sum += v;
            CHECK(index < sizeof(expect)/sizeof(expect[0]));
            CHECK(v == expect[index++]);
            std::cout << v << " ";
        }
        CHECK(index == sizeof(expect)/sizeof(expect[0]));
        CHECK(sum == 1 + 4 + 7);
    }
    {
        element_type_t sum = 0;
        size_t index = 0;
        for(auto v : rangex(1, 9, false, 3)) {
            static_assert(check_eq_typeof<decltype(v), element_type_t>());
            sum += v;
            CHECK(index < sizeof(expect)/sizeof(expect[0]));
            CHECK(v == expect[index++]);
            std::cout << v << " ";
        }
        CHECK(index == sizeof(expect)/sizeof(expect[0]));
        CHECK(sum == 1 + 4 + 7);
    }

    auto r = rangex(1, 9, false, 3);
    auto verify = [expect] (rangex<int, false> r) {
        element_type_t sum = 0;
        size_t index = 0;
        for(auto v : r) {
            static_assert(check_eq_typeof<decltype(v), element_type_t>());
            sum += v;
            CHECK(index < sizeof(expect)/sizeof(expect[0]));
            CHECK(v == expect[index++]);
            std::cout << v << " ";
        }
        CHECK(index == sizeof(expect)/sizeof(expect[0]));
        CHECK(sum == 1 + 4 + 7);
    };
    verify(r);
    verify(rangex(1, 9, true, 3));

    std::cout << std::endl;    
}

// if from <= to/through and step < 0, for loop will do nothing
// if from >= to/through and step > 0, for loop will do nothing
// if step == 0, Swift will panic. Should throw div by 0 exception or infinity loop?
TEST_CASE_EX(rangex_test, do_nothing_cases) {
    using element_type_t = uint8_t;
    element_type_t expect[] = {};
    auto verify = [expect] (rangex<element_type_t> r) {
        element_type_t sum = 0;
        size_t index = 0;
        for(auto v : r) {
            static_assert(check_eq_typeof<decltype(v), element_type_t>());
            sum += v;
            CHECK(index < sizeof(expect)/sizeof(expect[0]));
            CHECK(v == expect[index++]);
            std::cout << v << " ";
        }
        CHECK(index == sizeof(expect)/sizeof(expect[0]));
        CHECK(sum == 0);

        std::cout << std::endl;
    };
    verify(rangex<element_type_t>(1, 2, false, -1));
    verify(rangex<element_type_t>(1, 2, true, -1));
    verify(rangex<element_type_t>(1, 1, true, -1));
    verify(rangex<element_type_t>(2, 1, false, 1));
    verify(rangex<element_type_t>(2, 1, true, 1));
    verify(rangex<element_type_t>(2, 2, true, 1));
}


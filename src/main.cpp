#define TEST_FRAMEWORK_IMPLEMENT_WITH_MAIN
#include "test_framework.h"

#include <iostream>
#include <cstdint>
#include <type_traits>
#include <cassert>

#ifdef _MSC_VER
#define COMPILER_HAS_NO_STD_FLOAT
#endif

#include "rangex_lib.h"
using namespace ns_rangex;

// test_framework provides main()
TEST_CASE_EX(rangex_test, show_compiler_info) {
    printCompilerInfo();

    std::cout << "Compiler " << (checkCompilerHasStdFloat() ? "HAS" : "has NO") << " stdfloat support." << std::endl;
    std::cout << "COMPILER_HAS_STD_FLOAT " 
#ifdef COMPILER_HAS_STD_FLOAT
    << ""
#else
    << " NOT "
#endif
    << "defined." << std::endl;
}

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
        //std::cout << v << " ";
    }
    CHECK_EQ(index, sizeof(expect)/sizeof(expect[0]));
    CHECK_EQ(sum, 1 + 2 + 3 + 4 + 5);
    //std::cout << std::endl;
}

TEST_CASE_EX(rangex_test, default_exclusive_range_of_float) {
    // default rangex<int, false> as normal exclusive loop at step 1
    // get 1...5

    using element_type_t = std::float32_t;
    constexpr int element_type_bits = 32;
    element_type_t sum = scf<element_type_bits>(0.0f);
    element_type_t expect[] = {scf<element_type_bits>(1.0f), scf<element_type_bits>(2.0f), scf<element_type_bits>(3.0f), scf<element_type_bits>(4.0f), scf<element_type_bits>(5.0f)};
    size_t index = 0;
    for(auto v : rangex<element_type_t, false>(scf<element_type_bits>(1.0f), scf<element_type_bits>(6.0f))) {
        static_assert(check_eq_typeof<decltype(v), element_type_t>());
        sum += v;
        CHECK(index < sizeof(expect)/sizeof(expect[0]));
        CHECK(v == expect[index++]);
        //std::cout << v << " ";
    }
    CHECK_EQ(index, sizeof(expect)/sizeof(expect[0]));
    CHECK_EQ(sum, scf<element_type_bits>(1.0f) + scf<element_type_bits>(2.0f) + scf<element_type_bits>(3.0) + scf<element_type_bits>(4.0) + scf<element_type_bits>(5.0));
    //std::cout << std::endl;
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
        //std::cout << v << " ";
    }
    CHECK(index == 5);
    CHECK(sum == 1 + 2 + 3 + 4 + 5);
    //std::cout << std::endl;
}

TEST_CASE_EX(rangex_test, inclusive_range_of_float) {
    // float inclusive step 1
    // get 1...5

    using element_type_t = std::float32_t;
    constexpr int element_type_bits = 32;
    element_type_t sum = scf<element_type_bits>(0.0f);
    element_type_t expect[] = {scf<element_type_bits>(1.0f), scf<element_type_bits>(2.0f), scf<element_type_bits>(3.0f), scf<element_type_bits>(4.0f), scf<element_type_bits>(5.0f)};
    size_t index = 0;
    for(auto v : rangex<element_type_t, false>(scf<element_type_bits>(1), scf<element_type_bits>(5), true)) {
        static_assert(check_eq_typeof<decltype(v), element_type_t>());
        sum += v;
        CHECK(index < 5);
        CHECK(v == expect[index++]);
        //std::cout << v << " ";
    }
    CHECK(index == 5);
    CHECK(sum == scf<element_type_bits>(1.0f) + scf<element_type_bits>(2.0f) + scf<element_type_bits>(3.0f) + scf<element_type_bits>(4.0f) + scf<element_type_bits>(5.0f));
    //std::cout << std::endl;
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
        //std::cout << v << " ";
    }
    CHECK(index == 5);
    CHECK(sum == 5 + 4 + 3 + 2 + 1);
    //std::cout << std::endl;
}

template <typename T>
void verify_for_loop_range(T expect[], size_t expect_len, rangex<T, false> r) {
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

template <typename T>
void verify_for_loop_indexed_range(T expect[], size_t expect_len, rangex<T, true> r) {
    T sum = 0;
    size_t index = 0;
    for(auto iv : r) {
        static_assert(check_eq_typeof<decltype(iv.second), T>());
        static_assert(check_eq_typeof<decltype(iv.first), size_t>());
        sum += iv.second;
        CHECK(index == iv.first);
        CHECK(index < expect_len);
        CHECK_EQ(iv.second, expect[index++]);
    }
    CHECK(index == expect_len);
}

TEST_CASE_EX(rangex_test, inclusive_range_downward_of_float_default) {
    // float inclusive step -1
    // get 5...1 by -1
    using element_type_t = std::float32_t;
    constexpr int element_type_bits = 32;
    element_type_t expect[] = {scf<element_type_bits>(5.0f), scf<element_type_bits>(4.0f), scf<element_type_bits>(3.0f), scf<element_type_bits>(2.0f), scf<element_type_bits>(1.0f)};
    verify_for_loop_range<element_type_t>(expect, sizeof(expect)/sizeof(expect[0]), rangex<element_type_t, false>(scf<element_type_bits>(5.0f), scf<element_type_bits>(1.0f), true, scf<element_type_bits>(-1.0f)));
}

TEST_CASE_EX(rangex_test, inclusive_range_downward_of_int_explicitly) {
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
        //std::cout << v << " ";
    }
    CHECK(index == 5);
    CHECK(sum == 5 + 4 + 3 + 2 + 1);
    //std::cout << std::endl;
}

TEST_CASE_EX(rangex_test, inclusive_range_downward_of_float_explicitly) {
    // float inclusive step -1
    // get 5...1 by -1
    using element_type_t = std::float32_t;
    constexpr int element_type_bits = 32;
    element_type_t expect[] = {scf<element_type_bits>(5.0f), scf<element_type_bits>(4.0f), scf<element_type_bits>(3.0f), scf<element_type_bits>(2.0f), scf<element_type_bits>(1.0f)};
    verify_for_loop_range<element_type_t>(expect, sizeof(expect)/sizeof(expect[0]), rangex<element_type_t, false>(scf<element_type_bits>(5.0f), scf<element_type_bits>(1.0f), true, scf<element_type_bits>(-1.0f)));
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
        //std::cout << v << " ";
    }
    CHECK(index == 5);
    CHECK(sum == 5 + 4 + 3 + 2 + 1);
    //std::cout << std::endl;
}

TEST_CASE_EX(rangex_test, inclusive_range_downward_of_float64_t_explicitly_no_index_explicitly) {
    // int inclusive step -1
    // get 5...1 by -1

    using element_type_t = std::float64_t;
    constexpr int element_type_bits = 64;
    element_type_t expect[] = {scf<element_type_bits>(5.0f), scf<element_type_bits>(4.0f), scf<element_type_bits>(3.0f), scf<element_type_bits>(2.0f), scf<element_type_bits>(1.0f)};
    verify_for_loop_range<element_type_t>(expect, sizeof(expect)/sizeof(expect[0]), rangex<element_type_t, false>(scf<element_type_bits>(5.0f), scf<element_type_bits>(1.0f), true, scf<element_type_bits>(-1.0f)));
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
        //std::cout << v << " ";
    }
    CHECK(index == sizeof(expect)/sizeof(expect[0]));
    CHECK(sum == 5 + 4 + 3 + 2 + 1 + 0);
    //std::cout << std::endl;
}

#if !defined( COMPILER_HAS_STD_FLOAT ) || defined (COMPILER_HAS_NO_STD_FLOAT)
TEST_CASE_EX(SkipTest, inclusive_indexed_range_downward_of_typename_float16_t) {
    GTEST_SKIP() << "Wait half float support std::float16_t";
    // uint8_t inclusive step -1
    // get 5...1

    // using element_type_t = std::float16_t;
    // constexpr int element_type_bits = 16;
    // element_type_t expect[] = {scf<element_type_bits>(5.0f), scf<element_type_bits>(4.0f), scf<element_type_bits>(3.0f), scf<element_type_bits>(2.0f), scf<element_type_bits>(1.0f)};
    // verify_for_loop_indexed_range<element_type_t>(expect, sizeof(expect)/sizeof(expect[0]), rangex<element_type_t, true>(scf<element_type_bits>(5.0f), scf<element_type_bits>(1.0f), true, scf<element_type_bits>(-1.0f)));
}
#else
TEST_CASE_EX(rangex_test, inclusive_indexed_range_downward_of_typename_float16_t) {
    // uint8_t inclusive step -1
    // get 5...1

    using element_type_t = std::float16_t;
    element_type_t expect[] = {scf<16>(5.0f), scf<16>(4.0f), scf<16>(3.0f), scf<16>(2.0f), scf<16>(1.0f)};
    verify_for_loop_indexed_range<element_type_t>(expect, sizeof(expect)/sizeof(expect[0]), rangex<element_type_t, true>(scf<16>(5.0f), scf<16>(1.0f), true, scf<16>(-1.0f)));
}
#endif

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
            //std::cout << v << " ";
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
            //std::cout << v << " ";
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
            //std::cout << v << " ";
        }
        CHECK(index == sizeof(expect)/sizeof(expect[0]));
        CHECK(sum == 1 + 4 + 7);
    };
    verify(r);
    verify(rangex(1, 9, true, 3));

    //std::cout << std::endl;    
}

TEST_CASE_EX(rangex_test, step_gt_1_and_how_to_though_end_not_just_stepped_for_float) {
    using element_type_t = std::float32_t;
    //element_type_t expect[] = {scf<32>(5.0f), scf<32>(3.0f), scf<32>(1.0f)};
    element_type_t expect[] = {scf<32>(1.0f), scf<32>(4.0f), scf<32>(7.0f)};
    verify_for_loop_range<element_type_t>(expect, sizeof(expect)/sizeof(expect[0]), rangex<element_type_t, false>(scf<32>(1.0f), scf<32>(9.0f), false, scf<32>(3.0f)));
    verify_for_loop_range<element_type_t>(expect, sizeof(expect)/sizeof(expect[0]), rangex<element_type_t, false>(scf<32>(1.0f), scf<32>(9.0f), true, scf<32>(3.0f)));
}

// if from <= to/through and step < 0, for loop will do nothing
// if from >= to/through and step > 0, for loop will do nothing
// if step == 0, Swift will panic. Should throw div by 0 exception or infinity loop?
TEST_CASE_EX(rangex_test, do_nothing_cases) {
    using element_type_t = uint8_t;
    // Compatability: MSVC can't create zero element, place a stub 0
    element_type_t expect[] = { 0 };
    auto verify = [expect] (rangex<element_type_t> r) {
        element_type_t sum = 0;
        size_t index = 0;
        for(auto v : r) {
            static_assert(check_eq_typeof<decltype(v), element_type_t>());
            sum += v;
            CHECK(index < sizeof(expect)/sizeof(expect[0]) - 1);
            CHECK(v == expect[1 + index++]);
            //std::cout << v << " ";
        }
        CHECK(index == sizeof(expect)/sizeof(expect[0]) - 1);
        CHECK(sum == 0);

        //std::cout << std::endl;
    };
    verify(rangex<element_type_t>(1, 2, false, -1));
    verify(rangex<element_type_t>(1, 2, true, -1));
    verify(rangex<element_type_t>(1, 1, true, -1));
    verify(rangex<element_type_t>(2, 1, false, 1));
    verify(rangex<element_type_t>(2, 1, true, 1));
    verify(rangex<element_type_t>(2, 2, true, 1));

    {
        // if from <= to/through and step < 0, for loop will do nothing
        // if from >= to/through and step > 0, for loop will do nothing
        // if step == 0, Swift will panic. Should throw div by 0 exception or infinity loop?
        using element_type_t = std::float32_t;
        //element_type_t expect[] = {scf<32>(5.0f), scf<32>(3.0f), scf<32>(1.0f)};
        // Compatability: MSVC can't create zero element, place a stub 0
        element_type_t expect[] = { scf<32>(0.0f) };
        verify_for_loop_range<element_type_t>(expect, sizeof(expect)/sizeof(expect[0]) - 1, rangex<element_type_t, false>(scf<32>(1.0f), scf<32>(2.0f), false, scf<32>(-1.0f)));
        verify_for_loop_range<element_type_t>(expect, sizeof(expect)/sizeof(expect[0]) - 1, rangex<element_type_t, false>(scf<32>(1.0f), scf<32>(2.0f), true, scf<32>(-1.0f)));
        verify_for_loop_range<element_type_t>(expect, sizeof(expect)/sizeof(expect[0]) - 1, rangex<element_type_t, false>(scf<32>(2.0f), scf<32>(1.0f), false, scf<32>(1.0f)));
        verify_for_loop_range<element_type_t>(expect, sizeof(expect)/sizeof(expect[0]) - 1, rangex<element_type_t, false>(scf<32>(2.0f), scf<32>(1.0f), true, scf<32>(1.0f)));

        //behaviour under step is 0 is undefined
        //verify_for_loop_range<element_type_t>(expect, sizeof(expect)/sizeof(expect[0]), rangex<element_type_t, false>(scf<32>(1.0f), scf<32>(2.0f), false, scf<32>(0.0f)));
        //verify_for_loop_range<element_type_t>(expect, sizeof(expect)/sizeof(expect[0]), rangex<element_type_t, false>(scf<32>(2.0f), scf<32>(1.0f), true, scf<32>(0.0f)));
    }
}

// std:fmod/floor etc, do not support float128
TEST_CASE_EX(SkipTest, float128_cause_trouble) {
    GTEST_SKIP() << "Wait for better std::float128_t support";
    //using element_type_t = std::float128_t;
    using element_type_t = std::float64_t;
    // Compatability: MSVC can't create zero element, place a stub 0
    element_type_t expect[] = { 0.0 };
    constexpr int float_bits = 64; //128
    verify_for_loop_range<element_type_t>(expect, sizeof(expect)/sizeof(expect[0]) - 1, rangex<element_type_t, false>(scf<float_bits>(1.0f), scf<float_bits>(2.0f), true, scf<float_bits>(0.5f)));
}

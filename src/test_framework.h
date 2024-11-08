// test_framework.h
#pragma once

// Define which testing framework to use
//#define USE_GOOGLE_TEST
//#define USE_DOC_TEST

#if !defined(USE_GOOGLE_TEST) && !defined(USE_DOC_TEST)
#define USE_NON_TEST
#endif

#ifdef USE_NON_TEST
void do_nothing (bool) {} 
#define TEST_CASE_EX(ts, name) fn ts#name()
#define CHECK(exp) do_nothing(exp)

#ifdef TEST_FRAMEWORK_IMPLEMENT_WITH_MAIN
int main(int argc, char **argv) {
    //::testing::InitGoogleTest(&argc, argv);
    //return RUN_ALL_TESTS();
}
#endif
#endif

#ifdef USE_GOOGLE_TEST
#include <gtest/gtest.h>
#define TEST_CASE_EX(ts, name) TEST(ts, name)
#define CHECK(exp) ASSERT_TRUE(exp)
#define CHECK_EQ(val1, val2) EXPECT_EQ(val1, val2)
#define CHECK_NE(val1, val2) EXPECT_NE(val1, val2)
#include <gtest/gtest.h>
#ifdef TEST_FRAMEWORK_IMPLEMENT_WITH_MAIN
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
#endif
#endif

#ifdef USE_DOC_TEST
#include <doctest/doctest.h>
#ifdef TEST_FRAMEWORK_IMPLEMENT_WITH_MAIN
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#endif
#define TEST_CASE_EX(ts, name) TEST_CASE(#name)
//#define CHECK_EQ(val1, val2) CHECK(val1 == val2)
//#define CHECK_NE(val1, val2) CHECK(val1 != val2)
#endif

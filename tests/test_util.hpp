#pragma once

#include <iostream>
#include <string>
#include <cstdlib>
#include <chrono>

#include "lib/utils/colors_unicodes.hpp"

/**
 * @brief Generates main method.
 */
#define TESTS_BEGIN int main(int, char**) { int _test_iterator_ = 0;

#define TESTS_END PRINT_FINISHED; return 0; }

#define PRINT_STARTED(name) std::cout << SET_COLOR_BLUE "[INFO] Test #" \
        << _test_iterator_ << ": " << (name) << " started." << COLOR_RESET \
        << std::endl;


#define PRINT_FINISHED if (_test_iterator_ != 0) { \
            std::cout << SET_COLOR_GREEN << "[ OK ] Test #" << _test_iterator_\
            << " finished." << COLOR_RESET << std::endl << std::endl; \
        } \
        _test_iterator_++;

/**
 * @brief Names the next block of code and prints information about it
 */
#define TEST(name) PRINT_FINISHED PRINT_STARTED(name)

#define TIMER_START { auto start = std::chrono::high_resolution_clock::now(); 

#define TIMER_STOP auto end = std::chrono::high_resolution_clock::now();\
    std::chrono::duration<double> elapsed = end - start;\
    std::cout << SET_COLOR_GREEN << "[TIME] " << elapsed.count() << " ms" \
        << COLOR_RESET << std::endl; }

#define PRINT_TEST_FAILED_MSG(msg) std::cout << SET_COLOR_RED "[FAIL] Test #" \
    << _test_iterator_ << " failed at line " << __LINE__ << ": " << msg \
    << COLOR_RESET << std::endl;

/**
 * @brief For your test failure cases.
 */
#define RETURN_ERR_CODE return 1;

/**
 * @brief If expected isn't equals actual main returns 1.
 */
#define ASSERT_EQUAL(expected, actual) { \
        auto _expected_value_ = (expected); \
        auto _actual_value_ = (actual); \
        if (_expected_value_ != _actual_value_) { \
            PRINT_TEST_FAILED_MSG("expected " << _expected_value_ << ", got " << _actual_value_) \
            RETURN_ERR_CODE \
        } \
    }

/**
 * @brief In case of false main returns 1.
 */
#define ASSERT_TRUE(condition) \
    if (!(condition)) { \
        PRINT_TEST_FAILED_MSG("condition is false, expected true") \
        RETURN_ERR_CODE \
    }

/**
 * @brief In case of true main returns 1.
 */
#define ASSERT_FALSE(condition) \
    if (condition) { \
        PRINT_TEST_FAILED_MSG("condition is true, expected false") \
        RETURN_ERR_CODE \
    }

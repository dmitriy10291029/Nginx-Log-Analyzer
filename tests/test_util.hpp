#include <iostream>
#include <string>
#include <cstdlib>


#define COLOR_RESET        "\033[0m"
#define SET_COLOR_RED      "\033[31m"
#define SET_COLOR_GREEN    "\033[32m"
#define SET_COLOR_BLUE     "\033[34m"


#define TEST_SCOPE(tests) int main(int, char**) {\
        int _test_iterator_ = 0; \
        tests; \
        PRINT_FINISHED; \
        return 0;\
    }


#define PRINT_STARTED(name) std::cout << SET_COLOR_BLUE "[INFO] Test #" \
        << _test_iterator_ << ": " << (name) << " started." << COLOR_RESET \
        << std::endl;


#define PRINT_FINISHED if (_test_iterator_ != 0) { \
            std::cout << SET_COLOR_GREEN << "[ OK ] Test #" << _test_iterator_\
            << " finished." << COLOR_RESET << std::endl << std::endl; \
        } \
        _test_iterator_++;


#define TEST(name) PRINT_FINISHED PRINT_STARTED(name)


#define PRINT_TEST_FAILED_MSG(msg) std::cout << SET_COLOR_RED "[FAIL] Test #" \
    << _test_iterator_ << " failed at line " << __LINE__ << ": " << msg \
    << COLOR_RESET << std::endl;


#define RETURN_ERR_CODE return 1;


#define ASSERT_EQUAL(expected, actual) { \
        auto _expected_value_ = (expected); \
        auto _actual_value_ = (actual); \
        if (_expected_value_ != _actual_value_) { \
            PRINT_TEST_FAILED_MSG("expected " << _expected_value_ << ", got " << _actual_value_) \
            RETURN_ERR_CODE \
        } \
    }


#define ASSERT_TRUE(condition) \
    if (!(condition)) { \
        PRINT_TEST_FAILED_MSG("condition is false, expected true") \
        RETURN_ERR_CODE \
    }


#define ASSERT_FALSE(condition) \
    if (condition) { \
        PRINT_TEST_FAILED_MSG("condition is true, expected false") \
        RETURN_ERR_CODE \
    }

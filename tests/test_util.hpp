#include <iostream>
#include <string>
#include <cstdlib>

#define PRINT_TEST_FAILED_MSG(msg) std::cerr << "[FAIL] Line " << \
    __LINE__ << ": " << msg << std::endl;

#define RETURN_ERR_CODE std::exit(1);

#define ASSERT_EQUAL(expected, actual) \
    if ((expected) != (actual)) { \
        PRINT_TEST_FAILED_MSG("expected " << (expected) << \", but got " << \
        (actual)) \
        RETURN_ERR_CODE \
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

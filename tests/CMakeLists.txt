add_library(test_util_library STATIC test_util.hpp)
set_target_properties(test_util_library PROPERTIES LINKER_LANGUAGE CXX)
enable_testing()

# time_util_test
add_executable(time_util_test time_util_test.cpp)
target_link_libraries(time_util_test
    PRIVATE test_util_library
    PRIVATE utils_library
)
target_include_directories(time_util_test PRIVATE ${PROJECT_SOURCE_DIR})
add_test(NAME TIME_UTIL_TEST COMMAND time_util_test)

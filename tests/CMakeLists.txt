add_library(test_util_library STATIC test_util.hpp)
set_target_properties(test_util_library PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(test_util_library
    PRIVATE utils_library
)
target_include_directories(test_util_library PRIVATE ${PROJECT_SOURCE_DIR})
enable_testing()

# time_util_test
add_executable(time_util_test time_util_test.cpp)
target_link_libraries(time_util_test
    PRIVATE test_util_library
    PRIVATE utils_library
)
target_include_directories(time_util_test PRIVATE ${PROJECT_SOURCE_DIR})
add_test(NAME TIME_UTIL_TEST COMMAND time_util_test)


# args_parser_test
add_executable(args_parser_test args_parser_test.cpp)
target_link_libraries(args_parser_test
    PRIVATE test_util_library
    PRIVATE args_parser_library
)
target_include_directories(args_parser_test PRIVATE ${PROJECT_SOURCE_DIR})
add_test(NAME ARGS_PARSER_TEST COMMAND args_parser_test)

# log_analyzer_test
add_executable(log_analyzer_test log_analyzer_test.cpp)
target_link_libraries(log_analyzer_test
    PRIVATE test_util_library
    PRIVATE log_analyzer_library
)
target_include_directories(log_analyzer_test PRIVATE ${PROJECT_SOURCE_DIR})
add_test(NAME LOG_ANALYZER_TEST COMMAND log_analyzer_test)


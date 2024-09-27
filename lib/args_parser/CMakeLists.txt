add_library(args_parser_library STATIC
    args_parser.cpp
    args_parser.hpp
    parsing_result.hpp
)

target_link_libraries(args_parser_library
    PRIVATE utils_library
)
target_include_directories(args_parser_library PRIVATE ${PROJECT_SOURCE_DIR})
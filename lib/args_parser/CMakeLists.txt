add_library(args_parser_library STATIC
    args_parser.cpp
    args_parser.hpp
)

target_include_directories(args_parser_library PUBLIC log_analyzer_library)
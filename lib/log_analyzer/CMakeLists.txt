add_library(log_analyzer_library STATIC
    log_analyzer.cpp
    log_analyzer.hpp
    request.hpp
)

target_include_directories(log_analyzer_library PUBLIC ${PROJECT_SOURCE_DIR})

target_link_libraries(log_analyzer_library 
    PRIVATE args_parser_library
    PRIVATE utils_library
)
add_executable(AnalyzeLog main.cpp)

target_link_libraries(${PROJECT_NAME} PUBLIC log_analyzer_library)

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR})
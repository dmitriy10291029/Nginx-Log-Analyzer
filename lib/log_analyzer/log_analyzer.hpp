#pragma once

#include "lib/args_parser/args_parser.hpp"

class LogAnalyzer {
private:
    ArgsParser args_parser_;

public:
    explicit LogAnalyzer();

    int Run(char** argv);
};
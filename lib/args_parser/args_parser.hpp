#pragma once

#include "parsing_result.hpp"

class ArgsParser {
private:


public:
    ArgsParser();
    
    ParsingResult Parse(int argc, char** argv);
};
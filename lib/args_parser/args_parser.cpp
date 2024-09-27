#include "args_parser.hpp"

#include <cstring>


ArgsParser::ArgsParser() {
    
}

ParsingResult ArgsParser::Parse(int argc, char** argv) {
    ParsingResult parsing_result;

    for (int i = 1; i < argc; ++i) {
        if (argv[i][0] != '-') {
            
        } else if (argv[i][1] != '-') {

        } else {

        }
    }
}



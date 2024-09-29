#include <iostream>

#include "log_analyzer.hpp"
#include "lib/utils/print_util.hpp" 


LogAnalyzer::LogAnalyzer() {
    args_parser_.AddFlag("help", 'h')
        .AddFlag("print", 'p')
        .AddOption("output", 'o')
        .AddOption("stats", 's')
        .AddOption("window", 'w')
        .AddOption("from", 'f')
        .AddOption("to", 'e');
}

int LogAnalyzer::Run(int argc, char** argv) {
    int err_code = args_parser_.Parse(argc, argv);
    
    if (args_parser_.GetFlag("help").value().declared) {
        PrintUtil::PrintlnInfo(USAGE_TEXT);
    }

    return err_code;
}
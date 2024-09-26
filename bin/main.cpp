#include <iostream>

#include "lib/log_analyzer/log_analyzer.hpp"

int main(int argc, char** argv) {
    LogAnalyzer log_analyzer;
    
    return log_analyzer.Run(argv);
}

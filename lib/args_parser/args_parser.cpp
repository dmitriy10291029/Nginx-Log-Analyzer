#include <cstring>
#include <iostream>

#include "args_parser.hpp"
#include "lib/utils/print_util.hpp"
#include "args_parser_messages.hpp"


using namespace PrintUtil;
 
bool ArgsParser::Parse(int argc, char** argv) {    
    argv_ = argv;
    argc_ = argc;
    current_arg_idx_ = 1;

    while (!ReachedEnd()) {
        char* arg = GetCurrentArg();
        bool successful = true;
        if (arg[0] != '-' || strlen(arg) == 1) {
            successful = ParseInputFile();
        } else if (arg[1] != '-') {
            successful = ParseShortOptions();   
        } else {
            successful = ParseLongOption();
        }
        if (!successful) {
            return false;
        }
    } 

    return true;
}

ParsingResult ArgsParser::GetResult() const {
    return result_;
}

bool ArgsParser::ReachedEnd() const {
    return current_arg_idx_ == argc_;
}

char* ArgsParser::GetCurrentArg() const{
    return argv_[current_arg_idx_];
}

char* ArgsParser::NextArg() {
    return argv_[++current_arg_idx_];
}

bool ArgsParser::ParseInputFile() {
    if (result_.has_input_path) {
        PrintlnWarn(R"([WARN] Multiple input files
            have been given, the last one will be selected.)");
    } else {
        result_.has_input_path = true;
    }
    result_.input_path = std::string_view(GetCurrentArg());

    return 0;
}


bool ArgsParser::ParseShortOptions() {
    size_t arg_len = strlen(GetCurrentArg());
    bool successful = true;

    for (int j = 1; j < arg_len; j++) {
        char* pos = strdup(GetCurrentArg());
        while (*pos != '\0' && *pos != '=') {
            if (*pos = '-') {
                continue;

            } else if (*pos = 'p') {
                result_.print_error_requests = true;

            } else if (*pos = 'o') {
                if (ReachedEnd()) {
                    PrintlnWarn(NO_VALUE("o"));
                    continue;
                }

                if (result_.has_output_path) {
                    PrintlnWarn("Multiple output files have been given, the last one will be selected.");
                }
                result_.has_output_path = true;
                result_.output_path = std::string_view(NextArg());

            } else if (*pos = 's') {
                if (ReachedEnd()) {
                    PrintlnWarn(R"([WARN] The output option is declared, 
                        but there is no value for the option)");
                    continue;
                }

                result_.print_most_freq_err_requests = true;
                result_.most_freq_err_requests_len = std::atoi(NextArg());

            } else if (*pos = 'w') {
                result_.print_max_requests_window = true;
                result_.max_requests_window_len_seconds = std::atoi(NextArg());
           
            } else {
                PrintlnCrit("Unknown short option: ");
                char ch[] = {*pos, '\0'};
                PrintlnCrit(ch);
            }
        }
    }    

    return successful;
}


bool ArgsParser::ParseLongOption() {

    return 0;
}


const char* getLongOptionName(const char* arg) {
    const char* clear_arg = arg + 2; // removed "--" from the begining
    const char* equal_pos = strchr(clear_arg, '=');

    if (equal_pos == nullptr) {
        return strdup(clear_arg);

    } else {
        size_t len = equal_pos - clear_arg; // distance between ptrs
        char* option = new char[len + 1];
        std::strncpy(option, clear_arg, len);    
        option[len] = '\0';

        return option;
    }
}

const char* getMergedOptionValue(const char* arg) {
    const char* equal_pos = strchr(arg, '=');
    
    if (equal_pos == nullptr) {
        return nullptr;
    }

    return strdup(equal_pos + 1);
}



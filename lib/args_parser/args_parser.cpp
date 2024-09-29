#include <cstring>
#include <iostream>

#include "args_parser.hpp"
#include "lib/utils/print_util.hpp"
#include "args_parser_messages.hpp"


using namespace PrintUtil;

ArgsParser& ArgsParser::AddFlag(const char* long_name, char short_name) {
    if (flags_size_ == maxAmountOfOptions) {
        PrintlnWarn(FLAGS_FULL);
        return *this;
    }
    if (IsAlreadyDeclared(long_name, short_name)) {
        PrintlnWarn(NAMES_REPEATED);
        return *this;
    }
    flags_[flags_size_].long_name = long_name;
    flags_[flags_size_].short_name = short_name;
    ++flags_size_;

    return *this;
}

ArgsParser& ArgsParser::AddOption(const char* long_name, char short_name) {
    if (options_size_ == maxAmountOfOptions) {
        PrintlnWarn(OPTIONS_FULL);
        return *this;
    }
    if (IsAlreadyDeclared(long_name, short_name)) {
        PrintlnWarn(NAMES_REPEATED);
        return *this;
    }
    options_[options_size_].long_name = long_name;
    options_[options_size_].short_name = short_name;
    ++options_size_;

    return *this;
}
 
int ArgsParser::Parse(int argc, char** argv) {    
    err_code_ = 0;
    argv_ = argv;
    argc_ = argc;
    current_arg_idx_ = 1;
    next_arg_idx_ = 2;
    const char* arg = GetCurrentArg();

    while (!ReachedEnd()) {
        if (arg[0] != '-' || strlen(arg) == 1) {
            AddFreeArg(arg);
        } else if (arg[1] != '-') {
            ParseShortName();   
        } else {
            ParseLongName();
        }
        arg = NextArg();
    }

    return err_code_;
}

std::optional<Flag> ArgsParser::GetFlag(const char* long_name) const {
    for (size_t it = 0; it < flags_size_; it++) {
        if (strcmp(flags_[it].long_name, long_name) == 0) {
            return flags_[it];
        }    
    }

    return std::nullopt;
}

std::optional<Option> ArgsParser::GetOption(const char* long_name) const {
    for (size_t it = 0; it < options_size_; it++) {
        if (strcmp(options_[it].long_name, long_name) == 0) {
            return options_[it];
        }    
    }

    return std::nullopt;
}

char** ArgsParser::GetFreeArgs() const {
    return free_args_;
}

size_t ArgsParser::GerFreeArgsSize() const {
    return free_args_size_;
}


bool ArgsParser::IsAlreadyDeclared(const char* long_name, char short_name) const {
    for (size_t it = 0; it < flags_size_; it++) {
        if (strcmp(flags_[it].long_name, long_name) == 0 
            || flags_[it].short_name == short_name) {
            return true;
        }    
    }
    for (size_t it = 0; it < options_size_; it++) {
        if (strcmp(options_[it].long_name, long_name) == 0 
            || options_[it].short_name == short_name) {
            return true;
        }    
    }

    return false;
}

bool ArgsParser::ReachedEnd() const {
    return current_arg_idx_ >= argc_;
}

char* ArgsParser::GetCurrentArg() const{
    return argv_[current_arg_idx_];
}

char* ArgsParser::NextArg() {
    current_arg_idx_ = next_arg_idx_;
    next_arg_idx_++;

    return argv_[next_arg_idx_];
}

void ArgsParser::AddFreeArg(const char* arg) {
    if (free_args_size_ <= maxAmountOfFreeArgs) {
        free_args_[free_args_size_++] = strdup(arg);
    }
}


void ArgsParser::ParseShortName() {
    char* pos = strdup(GetCurrentArg() + 1); // skip '-'

    while (*pos != '\0' && *pos != '=') {
        if (ParseShortNameFlag(*pos) || ParseShortNameOption(*pos)) {
            ++pos;
            continue;
        }

        err_code_ = 1;
        const char str[] = {*pos, ':'}; 
        PrintlnCrit(str);

        if (std::isalpha(*pos)) {
            PrintlnCrit(UNKNOWN_NM);

        } else {
            PrintlnCrit(UNKNOWN_FORMAT);
        }

        ++pos;
    }    
}

bool ArgsParser::ParseShortNameFlag(const char name) {
    for (int it = 0; it < flags_size_; it++) {
        if (name == flags_[it].short_name) {
            flags_[it].declared = true;

            return true;
        }
    }
    
    return false;
}

bool ArgsParser::ParseShortNameOption(char name) {
    for (int it = 0; it < options_size_; it++) {
        if (name == options_[it].short_name) {
            options_[it].declared = true;
            auto value = GetValue(current_arg_idx_);

            if (value) {
                if (options_[it].value_declared) {
                    const char str[] = {name, ':'}; 
                    PrintlnWarn(str);
                    PrintlnWarn(MULTIPLE_VALUES);
                }
                options_[it].value = value.value();
                options_[it].value_declared = true;
            }

            return true;
        }
    }

    return false;
}

void ArgsParser::ParseLongName() {
    const char* name = GetLongOptionName(GetCurrentArg());

    if (ParseLongNameFlag(name) || ParseLongNameOption(name)) {
        return;
    }

    err_code_ = 1;
    PrintlnCrit(name);
    PrintlnCrit(UNKNOWN_NAME);
}
    
bool ArgsParser::ParseLongNameFlag(const char* name) {
    for (int it = 0; it < flags_size_; it++) {
        if (strcmp(name, flags_[it].long_name) == 0) {
            flags_[it].declared = true;

            return true;
        }
    }
    
    return false;
}

bool ArgsParser::ParseLongNameOption(const char* name) {
    for (int it = 0; it < options_size_; it++) {
        if (strcmp(name, options_[it].long_name) == 0) {
            options_[it].declared = true;
            auto value = GetValue(current_arg_idx_);

            if (value) {
                if (options_[it].value_declared) {
                    PrintlnWarn(MULTIPLE_VALUES); // to-do
                }
                options_[it].value = value.value();
                options_[it].value_declared = true;
            }

            return true;
        }
    }

    return false;
}

std::optional<const char*> ArgsParser::GetValue(size_t arg_idx) {
    const char* equal_pos = strchr(argv_[arg_idx], '=');
    if (equal_pos != nullptr) {
        return equal_pos + 1; // skip '='
    }

    if (next_arg_idx_ >= argc_) {
        return std::nullopt;
    }

    const char* next = argv_[next_arg_idx_];
    if (next[0] != '-') {
        next_arg_idx_++;
        
        return next;
    }

    return std::nullopt;
}

const char* ArgsParser::GetLongOptionName(const char* arg) const {
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

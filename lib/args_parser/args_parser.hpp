#pragma once

#include "parsing_result.hpp"

class ArgsParser {
public:
    [[nodiscard]] bool Parse(int argc, char** argv);
    [[nodiscard]] ParsingResult GetResult() const;

private:
    char**          argv_ = nullptr;
    int             argc_ = 0;
    size_t          current_arg_idx_ = 0;
    ParsingResult   result_ = ParsingResult();

    bool ReachedEnd() const;
    char* GetCurrentArg() const;
    char* NextArg();
    bool ParseShortOptions();
    bool ParseInputFile();
    bool ParseLongOption();
};
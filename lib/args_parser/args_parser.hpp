#pragma once

#include <optional>

#include "flag.hpp"
#include "option.hpp"


constexpr size_t maxAmountOfOptions     = 50;
constexpr size_t maxAmountOfFreeArgs    = 100;

class ArgsParser {
public:
    ArgsParser& AddFlag(const char* long_name, char short_name);
    ArgsParser& AddOption(const char* long_name, char short_name);

    [[nodiscard]] int Parse(int argc, char** argv);

    [[nodiscard]] std::optional<Flag>   GetFlag(const char* long_name) const;
    [[nodiscard]] std::optional<Option> GetOption(const char* long_name) const;

    [[nodiscard]] char** GetFreeArgs() const;
    [[nodiscard]] size_t GetFreeArgsSize() const;

private:
    Option* options_ = new Option[maxAmountOfOptions];
    size_t  options_size_ = 0;

    Flag*   flags_ = new Flag[maxAmountOfOptions];
    size_t  flags_size_ = 0;

    char**  free_args_ = new char*[maxAmountOfFreeArgs];
    size_t  free_args_size_ = 0;

    char**  argv_ = nullptr;
    int     argc_ = 0;
    size_t  current_arg_idx_ = 0;
    size_t  next_arg_idx_ = 0;

    int     err_code_ = 0;

    bool    IsAlreadyDeclared(const char* long_name, char short_name) const;

    bool    ReachedEnd() const;
    char*   GetCurrentArg() const;
    char*   NextArg();
    
    void    AddFreeArg(char* arg);

    void    ParseShortName();
    bool    ParseShortNameFlag(char name);
    bool    ParseShortNameOption(char name);
    
    void    ParseLongName();
    bool    ParseLongNameFlag(const char* name);
    bool    ParseLongNameOption(const char* name);

    std::optional<const char*>  GetValue(size_t arg_idx);
    const char*                 GetLongOptionName(const char* arg) const;
};
#pragma once

#include <cstdint>

#include "lib/args_parser/args_parser.hpp"
#include <fstream>

#define USAGE_TEXT \
"Usage: log_file [options]\n\
Options:\n\
  -o path, --output=path      Path to the file where failed requests will be logged. If the file is not specified, failed request analysis is not performed. \n\
  -p, --print                 Duplicate the failed request output \n\
  -s n, --stats=n             Output the `n` most frequent requests that ended with a `5XX` status code, sorted by their frequency. Default is `10`. \n\
  -w t, --window=t            Find and display the time window of `t` seconds during which the number of requests was at its peak. If `t` is `0`, no calculation is performed. Default is `0`. \n\
  -f time, --from=time        Start analyzing data from the specified time in timestamp format. Default is the earliest time in the log. \n\
  -e time, --to=time          Analyze data up to the specified time in timestamp format (inclusive). Default is the latest time in the log."

constexpr size_t maxBufferSize = 512;

class LogAnalyzer {
public:
    void SetUp(int argc, char** argv);

    [[nodiscard]] int Run(int argc, char** argv);

private:
    ArgsParser      args_parser_;

    bool            has_input_path_ = false;
    std::ifstream*  input_stream_ = nullptr;

    bool            has_output_path_ = false;
    std::ofstream*  output_stream_ = nullptr;
    
    bool            print_error_requests_ = false;
    
    bool            print_most_freq_err_requests_ = false;
    uint32_t        most_freq_err_requests_len_ = 10;

    bool            print_max_requests_window_ = false;
    uint32_t        max_requests_window_len_seconds_ = 0;

    bool            has_lower_time_bound_ = false;
    int64_t         lower_time_bound_unix_ = 0;

    bool            has_upper_time_bound_ = false;
    int64_t         upper_time_bound_unix_ = 0;
 
    char            buffer_[maxBufferSize];
    size_t          bufferSize_ = 0;

    int             err_code_ = 0;

    void SetUpHelp();
    void SetUpInput();
    void SetUpOutput();
    void SetUpPrint();
    void SetUpStats();
    void SetUpWindow();
    void SetUpFrom();
    void SetUpTo();

    bool ReadRequest();
};
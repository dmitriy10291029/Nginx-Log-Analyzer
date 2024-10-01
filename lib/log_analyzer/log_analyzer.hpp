#pragma once

#include <cstdint>
#include <fstream>
#include <unordered_map>
#include <queue>

#include "../args_parser/args_parser.hpp"
#include "request.hpp"

#define USAGE_TEXT \
"Usage: log_file [options]\n\
Options:\n\
  -o path, --output=path      Path to the file where failed requests will be logged. If the file is not specified, failed request analysis is not performed. \n\
  -p, --print                 Duplicate the failed request output \n\
  -s n, --stats=n             Output the `n` most frequent requests that ended with a `5XX` status code, sorted by their frequency. Default is `10`. \n\
  -w t, --window=t            Find and display the time window of `t` seconds during which the number of requests was at its peak. If `t` is `0`, no calculation is performed. Default is `0`. \n\
  -f time, --from=time        Start analyzing data from the specified time in timestamp format. Default is the earliest time in the log. \n\
  -e time, --to=time          Analyze data up to the specified time in timestamp format (inclusive). Default is the latest time in the log."

constexpr size_t    readBufferSize     = 1024 * 64;
constexpr size_t    writeBufferSize    = 1024 * 64;
constexpr size_t    lineLenInit        = 256;
constexpr size_t    lineLenMax         = 1024 * 64;

constexpr uint64_t  hashFuncModule     = 1e9 + 9;
constexpr uint64_t  hashFuncBase       = 31;

class LogAnalyzer {
public:
    void SetUp(int argc, char** argv);
    [[nodiscard]] int Run();
    [[nodiscard]] int GetTotalRequests() const;
    [[nodiscard]] int GetInvalidRequests() const;
    [[nodiscard]] int GetErrorRequests() const;

private:
    ArgsParser      args_parser_;

    bool            has_input_path_ = false;
    std::ifstream*  input_stream_ = nullptr;

    bool            has_output_path_ = false;
    std::ofstream*  output_stream_ = nullptr;
    
    bool            print_error_requests_ = false;
    
    bool            print_most_freq_err_requests_ = false;
    uint32_t        most_freq_err_requests_len_ = 10;
    std::fstream*   temp_file_ = nullptr;

    bool            print_max_requests_window_ = false;
    uint32_t        max_requests_window_len_seconds_ = 0;

    bool            has_lower_time_bound_ = false;
    int64_t         lower_time_bound_unix_ = 0;

    bool            has_upper_time_bound_ = false;
    int64_t         upper_time_bound_unix_ = 0;
 
    char            read_buffer_[readBufferSize];
    std::streamsize bytes_read_ = 0;
    size_t          read_buffer_pos_ = 0;

    char*           line_ = new char[lineLenInit];
    size_t          line_size_ = lineLenInit;
    size_t          line_len_ = 0;
    char*           line_pos_ = 0;
    Request         curr_request_;

    int             err_code_ = 0;
    int             err_amount_ = 0;
    int             requests_total_ = 0;
    int             invalid_amount_ = 0;
    bool            found_upper_time_bound_ = false;

    char            write_buffer_[writeBufferSize];
    size_t          write_buffer_pos_ = 0;

    std::unordered_multimap<
        uint64_t, std::pair<int, std::streampos>> req_freq_map;

    void            SetUpHelp();
    void            SetUpInput();
    void            SetUpOutput();
    void            SetUpPrint();
    void            SetUpStats();
    void            SetUpWindow();
    void            SetUpFrom();
    void            SetUpTo();
        
    void            ReadRequest();
    size_t          ReadNextLine();
    char            Get();
    bool            ReachedEnd();
        
    void            FindInLine(char target);
    void            FindInLineFromEnd(char target);
    void            SplitLine();
    void            IncrementLinePos();
    void            AssertUpperTimeBound();
    void            FindLowerTimeBound();
        
    bool            IsWhitespace(char ch);
    bool            IsCorrectStatus(const char* status);
    bool            IsServerErrorStatus(const char* status);
        
    void            Write(const char* text);
    void            WriteRequest();
    void            WriteRequestIfError();
    void            WriteBuffer();

    uint64_t        CalcStrHash(const char* str);
    std::streampos  TempFileAddRequest(const char* str);
    const char*     TempFileFind(std::streampos pos);
    bool            TempFileFindAndCompare(std::streampos pos, const char* str);
    void            PrintMostFreqErrReq();
};

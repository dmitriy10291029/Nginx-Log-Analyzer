#pragma once

#include "lib/args_parser/args_parser.hpp"

#define USAGE_TEXT \
"Usage: log_file [options]\n\
Options:\n\
  -o path, --output=path      Path to the file where failed requests will be logged. If the file is not specified, failed request analysis is not performed. \n\
  -p, --print                 Duplicate the failed request output \n\
  -s n, --stats=n             Output the `n` most frequent requests that ended with a `5XX` status code, sorted by their frequency. Default is `10`. \n\
  -w t, --window=t            Find and display the time window of `t` seconds during which the number of requests was at its peak. If `t` is `0`, no calculation is performed. Default is `0`. \n\
  -f time, --from=time        Start analyzing data from the specified time in timestamp format. Default is the earliest time in the log. \n\
  -e time, --to=time          Analyze data up to the specified time in timestamp format (inclusive). Default is the latest time in the log."


class LogAnalyzer {
private:
    ArgsParser args_parser_;

public:
    LogAnalyzer();

    [[nodiscard]] int Run(int argc, char** argv);
};
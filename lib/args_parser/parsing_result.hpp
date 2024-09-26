#pragma once

#include <string_view>
#include <limits>
#include <cstdint>

struct ParsingResult {

    bool                has_output_path = false;
    std::string_view    output_path;
    
    bool        print_error_requests = false;
    
    bool        print_most_freq_err_requests = false;
    uint32_t    most_freq_err_requests_len = 10;

    bool        print_max_requests_window = false;
    uint32_t    max_requests_window_len_seconds = 0;

    bool        has_lower_time_bound = false;
    int64_t     lower_time_bound_utc0 = 0;

    bool        has_upper_time_bound = false;
    int64_t     upper_time_bound_utc0 = std::numeric_limits<long>::max();
    
};

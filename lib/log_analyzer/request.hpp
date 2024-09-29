#pragma once

#include <cstdint>

struct Request {
    const char* remote_addr;
    int64_t datetime_unix;
    const char* url;
    uint16_t status_code;
};

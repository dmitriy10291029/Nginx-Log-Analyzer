#pragma once

#include <cstdint>

struct Request {
    char*       remote_addr     = nullptr;
    int64_t     datetime_unix   = 0;
    char*       url             = nullptr;
    uint32_t    status_code     = 0;
};

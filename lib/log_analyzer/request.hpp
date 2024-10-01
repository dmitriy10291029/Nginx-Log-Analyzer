#pragma once

#include <cstdint>

struct Request {
    char*       remote_addr     = nullptr;
    char*       datetime_unix    = nullptr;
    char*       url             = nullptr;
    char*       status_str      = nullptr;
    char*       bytes_str       = nullptr;
};

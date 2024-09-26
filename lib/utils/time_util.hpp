#pragma once

#include <cstdint>

#include "datetime.hpp"

namespace TimeUtil {
    const int64_t convertingErrorCode = -1;

    /**
     * @param datetime_cstr C-string in the format of logs (example: 
     * [01/Jul/1995:00:00:01 -0400])
     * 
     * @return in case of error returns convertingErrorCode, otherwise returns value in unix 
     * utc-0 format
     */
    int64_t ConvertLogsToUnixTime(const char* datetime_cstr);

} // namespace TimeUtil

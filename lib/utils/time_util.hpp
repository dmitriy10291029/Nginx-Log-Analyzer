#pragma once

#include <cstdint>

#include "datetime.hpp"

namespace TimeUtil {
    const int64_t convertingErrorCode = -1;

    /**
     * @param datetime_cstr C-string in the format of logs (example: 
     * [01/Jul/1995:00:00:01 -0400])
     * 
     * @return In case of error returns convertingErrorCode, otherwise returns value in unix 
     * utc-0 format
     */
    int64_t ConvertLogsTimeToUnix(const char* datetime_cstr);

    /**
     * @brief Checks, that timestamp c-string has digits, alphas, etc in the 
     * expected places;
     * 
     * @param dt datetime in logs format
     */
    bool IsCharsInDatetimeValid(const char* dt);

    /**
     * @param month_name name in Mmm format (ex.: Jan)
     * 
     * @return In case of error returns 0, otherwise numer starts from 1
     */
    uint8_t ConvertMonthNameToNumer(const char* month_name);


    uint8_t ConvertMonthNumerToDays(uint8_t numer);


    bool IsYearLeap(int year);


    const char* GetMonthNameFromDatetime(const char* datetime);


    int64_t ConvertDatetimeToUnix(DateTime dt);

} // namespace TimeUtil

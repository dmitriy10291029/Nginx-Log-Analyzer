#include "time_util.hpp"
#include "logs_format_values.hpp"

#include <cstring>
#include <cctype>
#include <iostream>


namespace TimeUtil {
    const int32_t daysPerMonth[] = {
        31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
    };

    const char* monthsNames[] = {
        "Jan", "Feb", "Mar", "Apr", "May", "Jun", 
        "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
    };

    int64_t ConvertLogsTimeToUnix(const char* logs_dt) {
        if (!IsCharsInDatetimeValid(logs_dt)) {
            return convertingErrorCode;
        }
        
        DateTime datetime;

        datetime.day = static_cast<uint8_t>(logs_dt[DAY_0_IDX] - '0') * 10 + 
                static_cast<uint8_t>(logs_dt[DAY_1_IDX] - '0');

        const char* month_str = GetMonthNameFromDatetime(logs_dt);        
        datetime.month = ConvertMonthNameToNumer(month_str);
        delete month_str;
        if (datetime.month == 0) {
            return convertingErrorCode;
        }

        datetime.year = static_cast<uint16_t>(logs_dt[YEAR_0_IDX] - '0') * 1000 + 
                static_cast<uint16_t>(logs_dt[YEAR_1_IDX] - '0') * 100 + 
                static_cast<uint16_t>(logs_dt[YEAR_2_IDX] - '0') * 10 + 
                static_cast<uint16_t>(logs_dt[YEAR_3_IDX] - '0');

        datetime.hours = static_cast<uint8_t>(logs_dt[HOURS_0_IDX] - '0') * 10 + 
                static_cast<uint8_t>(logs_dt[HOURS_1_IDX] - '0');

        datetime.minutes = static_cast<uint8_t>(logs_dt[MINUTES_0_IDX] - '0') * 10 + 
                static_cast<uint8_t>(logs_dt[MINUTES_1_IDX] - '0');

        datetime.seconds = static_cast<uint8_t>(logs_dt[SECONDS_0_IDX] - '0') * 10 + 
                static_cast<uint8_t>(logs_dt[SECONDS_1_IDX] - '0');

        if (logs_dt[TIMEZONE_SIGN_IDX] == '+') {
            datetime.timezone_sign = 1;
        } else if (logs_dt[TIMEZONE_SIGN_IDX] == '-') {
            datetime.timezone_sign = -1;
        } else {
            return -1;
        }

        datetime.timezone_hours = static_cast<uint8_t>(logs_dt[TIMEZONE_HOURS_0_IDX] - '0') * 10 + 
                static_cast<uint8_t>(logs_dt[TIMEZONE_HOURS_1_IDX] - '0');

        datetime.timezone_minutes = static_cast<uint8_t>(logs_dt[TIMEZONE_MINUTES_0_IDX] - '0') * 10 + 
                static_cast<uint8_t>(logs_dt[TIMEZONE_MINUTES_1_IDX] - '0');

        return ConvertDatetimeToUnix(datetime);
    }

    bool IsCharsInDatetimeValid(const char* dt) {
        return strlen(dt) != LOGS_FORMAT_SIZE ? false :
                dt[OPEN_BRACKET_IDX] == '[' &&
                isdigit(dt[DAY_0_IDX]) &&
                isdigit(dt[DAY_1_IDX]) &&
                dt[SLASH_0_IDX] == '/' &&
                isalpha(dt[MONTH_0_IDX]) &&
                isalpha(dt[MONTH_1_IDX]) &&
                isalpha(dt[MONTH_2_IDX]) &&
                dt[SLASH_1_IDX] == '/' &&
                isdigit(dt[YEAR_0_IDX]) &&
                isdigit(dt[YEAR_1_IDX]) &&
                isdigit(dt[YEAR_2_IDX]) &&
                isdigit(dt[YEAR_3_IDX]) &&
                dt[PUNCTUATION_0_IDX] == ':' &&
                isdigit(dt[HOURS_0_IDX]) &&
                isdigit(dt[HOURS_1_IDX]) &&
                dt[PUNCTUATION_1_IDX] == ':' &&
                isdigit(dt[MINUTES_0_IDX]) && 
                isdigit(dt[MINUTES_1_IDX]) &&
                dt[PUNCTUATION_2_IDX] == ':' &&
                isdigit(dt[SECONDS_0_IDX]) &&
                isdigit(dt[SECONDS_1_IDX]) &&
                dt[SPACE_IDX] == ' ' &&
                (dt[TIMEZONE_SIGN_IDX == '+'] || dt[TIMEZONE_SIGN_IDX] == '-') &&
                isdigit(dt[TIMEZONE_HOURS_0_IDX]) && 
                isdigit(dt[TIMEZONE_HOURS_1_IDX]) &&
                isdigit(dt[TIMEZONE_MINUTES_0_IDX]) &&
                isdigit(dt[TIMEZONE_MINUTES_1_IDX]) &&
                dt[CLOSE_BRACKET_IDX] == ']';
    }

    uint8_t ConvertMonthNameToNumer(const char* month_name) {
        uint8_t month_numer = 0;

        for (int i = 0; i < 12; ++i) {
            if (month_numer == 0 && strcmp(month_name, monthsNames[i]) == 0) {
                month_numer = i + 1; // month starts from 1, indexes from 0
            }
        }

        return month_numer;
    }

    int64_t ConvertDatetimeToUnix(DateTime dt) {
        int32_t days = dt.day - 1; // current day isn't done

        for (uint16_t y = 1970; y < dt.year; ++y) {
            days += IsYearLeap(y) ? 366 : 365;
        }

        for (uint8_t m = 0; m < dt.month - 1; ++m) {
            days += daysPerMonth[m];
        }

        if (dt.month > 1 && IsYearLeap(dt.year)) {  // 29th day of Feb
            days += 1;
        }

        int64_t timezone_offset_minutes = -dt.timezone_sign * 
                (dt.timezone_minutes + dt.timezone_hours * 60);  

        return static_cast<int64_t>(dt.seconds) + 
                (static_cast<int64_t>(dt.minutes) + timezone_offset_minutes) * 60 +
                static_cast<int64_t>(dt.hours) * 3600 +
                static_cast<int64_t>(days) * 86400;
    }

    uint8_t ConvertMonthNumerToDays(uint8_t numer) {
        return daysPerMonth[numer - 1]; // month starts from 1, indexes from 0
    }

    bool IsYearLeap(int year) {
        return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
    }

    const char* GetMonthNameFromDatetime(const char* datetime) {
        char* month_cstr = new char[4];
        month_cstr[0] = datetime[MONTH_0_IDX];
        month_cstr[1] = datetime[MONTH_1_IDX];
        month_cstr[2] = datetime[MONTH_2_IDX];
        month_cstr[3] = '\0';

        return month_cstr;
    }
} // namespace TimeUtil

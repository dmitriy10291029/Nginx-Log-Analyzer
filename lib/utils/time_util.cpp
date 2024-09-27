#include "time_util.hpp"
#include "logs_format_values.hpp"

#include <cstring>
#include <cctype>


namespace {
    using namespace TimeUtil;

    const int32_t daysPerMonth[] = {
        31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
    };

    const char* monthsNames[] = {
        "Jan", "Feb", "Mar", "Apr", "May", "Jun", 
        "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
    };

    /**
     * @brief Checks, that timestamp c-string has digits, alphas, etc in the 
     * expected places;
     * 
     * @param dt datetime in logs format
     */
    bool IsCharsInDatetimeValid(const char* dt) {
        return strlen(dt) == LOGS_FORMAT_SIZE &&
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

    const char* GetMonthNameFromDatetime(const char* datetime) {
        char* month_cstr = nullptr;
        strncpy(month_cstr, datetime + 4, 3);
        month_cstr[3] = '\0';

        return month_cstr;
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

    uint8_t ConvertMonthNumerToDays(uint8_t numer) {
        return daysPerMonth[numer - 1]; // month starts from 1, indexes from 0
    }

    bool IsYearLeap(int year) {
        return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
    }

    int64_t ConvertDatetimeToUnixTime(DateTime dt) {
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

        int64_t timezone_offset_minutes = dt.timezone_sign * 
                (dt.timezone_minutes + dt.timezone_hours * 60);  

        return static_cast<int64_t>(dt.seconds) + 
                (static_cast<int64_t>(dt.minutes) + timezone_offset_minutes) * 60 +
                static_cast<int64_t>(dt.hours) * 3600 +
                static_cast<int64_t>(days) * 86400;
    }
}

namespace TimeUtil {

    int64_t ConvertLogsTimeToUnix(const char* datetime_cstr) {
        if (!IsCharsInDatetimeValid(datetime_cstr)) {
            return convertingErrorCode;
        }
        
        DateTime datetime;

        datetime.day = datetime_cstr[DAY_0_IDX] * 10 + 
                datetime_cstr[DAY_1_IDX];

        datetime.month = ConvertMonthNameToNumer(
                GetMonthNameFromDatetime(datetime_cstr));
        if (datetime.month == 0) {
            return convertingErrorCode;
        }

        datetime.year = datetime_cstr[YEAR_0_IDX] * 1000 + 
                datetime_cstr[YEAR_1_IDX] * 100 + 
                datetime_cstr[YEAR_2_IDX] * 10 + 
                datetime_cstr[YEAR_3_IDX];
        
        datetime.hours = datetime_cstr[HOURS_0_IDX] * 10 + 
                datetime_cstr[HOURS_1_IDX];

        datetime.minutes = datetime_cstr[MINUTES_0_IDX] * 10 + 
                datetime_cstr[MINUTES_1_IDX];

        datetime.seconds = datetime_cstr[SECONDS_0_IDX] * 10 + 
                datetime_cstr[SECONDS_1_IDX];

        if (datetime_cstr[TIMEZONE_SIGN_IDX] == '+') {
            datetime.timezone_sign = 1;
        } else if (datetime_cstr[TIMEZONE_SIGN_IDX] == '-') {
            datetime.timezone_minutes = -1;
        } else {
            return -1;
        }

        datetime.timezone_hours = datetime_cstr[TIMEZONE_HOURS_0_IDX] * 10 + 
                datetime_cstr[TIMEZONE_HOURS_1_IDX];

        datetime.timezone_minutes = datetime_cstr[TIMEZONE_MINUTES_0_IDX] * 10 + 
                datetime_cstr[TIMEZONE_MINUTES_1_IDX];

        return ConvertDatetimeToUnixTime(datetime);
    }
} // namespace TimeUtil

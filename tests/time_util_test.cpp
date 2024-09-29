#include <iostream>
#include <cstdint>
#include <cstring>

#include "lib/utils/time_util.hpp"
#include "test_util.hpp"


using namespace TimeUtil;

TESTS_BEGIN

TEST("ConvertMonthNameToNumer") {
    ASSERT_EQUAL(ConvertMonthNameToNumer("Jan"), 1);
    ASSERT_EQUAL(ConvertMonthNameToNumer("Jun"), 6);
    ASSERT_EQUAL(ConvertMonthNameToNumer("Dec"), 12);
}

TEST("IsCharsInDatetimeValid") {
    ASSERT_TRUE(IsCharsInDatetimeValid("[01/Jul/1995:00:00:01 -0400]"));
    ASSERT_TRUE(IsCharsInDatetimeValid("[01/Jul/1995:00:00:01 +0400]"));

    ASSERT_FALSE(IsCharsInDatetimeValid("[01/Jul/1995:00:00:01+0400]"));
    ASSERT_FALSE(IsCharsInDatetimeValid("[01/Jul/1995:00:00:01 0400"));
    ASSERT_FALSE(IsCharsInDatetimeValid(""));
}

TEST("ConvertDatetimeToUnix") {
    DateTime dt;
    dt.day = 15;
    dt.month = 1;
    dt.year = 2004;
    dt.hours = 5;
    dt.minutes = 30;
    dt.seconds = 30;
    dt.timezone_sign = +1;
    dt.timezone_hours = 3;
    dt.timezone_minutes = 0;
    ASSERT_EQUAL(1074133830, ConvertDatetimeToUnix(dt))
}

TEST("GetMonthNameFromDatetime") {
    const char* actual = GetMonthNameFromDatetime("[01/Jul/1995:00:00:01 -0400]");
    ASSERT_TRUE(0 == strcmp("Jul", actual));
}

TEST("ConvertLogsTimeToUnix") {
    int64_t actual = ConvertLogsTimeToUnix("[01/Jul/1995:00:00:01 -0400]");
    ASSERT_EQUAL(804571201, actual);
}

TESTS_END

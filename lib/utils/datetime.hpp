#pragma once

#include <cstdint>


namespace TimeUtil {

    struct DateTime {
        int day = 0;
        int month = 0;
        int year = 0;
        int hours = 0;
        int minutes = 0;
        int seconds = 0;
        int timezone_sign = 1;
        int timezone_hours = 0;
        int timezone_minutes = 0;
    };

} // namespace TimeUtil

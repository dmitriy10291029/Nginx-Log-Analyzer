#pragma once

#include <cstdint>


namespace TimeUtil {

    struct DateTime {
        uint8_t     day = 0;
        uint8_t     month = 0;
        uint16_t    year = 0;
        uint8_t     hours = 0;
        uint8_t     minutes = 0;
        uint8_t     seconds = 0;
        int8_t      timezone_sign = 1;
        int8_t      timezone_hours = 0;
        int8_t      timezone_minutes = 0;
    };

} // namespace TimeUtil

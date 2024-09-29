#include <iostream>

#include "print_util.hpp"
#include "colors_unicodes.hpp"

namespace PrintUtil {
    void Println() {
        std::cout << std::endl;
    }

    void PrintlnInfo(const char* msg) {
        std::cout << SET_COLOR_BLUE << "[INFO] " << msg << COLOR_RESET << std::endl;
    }

    void PrintlnInfo(char ch) {
        std::cout << SET_COLOR_BLUE << "[INFO] " << ch << COLOR_RESET << std::endl;
    }

    void PrintlnWarn(const char* msg) {
        std::cout << SET_COLOR_YELLOW << "[WARN] " << msg << COLOR_RESET << std::endl;
    }

    void PrintlnWarn(char ch) {
        std::cout << SET_COLOR_YELLOW << "[WARN] " << ch << COLOR_RESET << std::endl;
    }

    void PrintlnCrit(const char* msg) {
        std::cout << SET_COLOR_RED << "[CRIT] " << msg << COLOR_RESET << std::endl;
    }

    void PrintlnCrit(char ch) {
        std::cout << SET_COLOR_RED << "[CRIT] " << ch << COLOR_RESET << std::endl;
    }

} // namespace PrintUtil

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

    void PrintlnInfo(int i) {
        std::cout << SET_COLOR_BLUE << "[INFO] " << i << COLOR_RESET << std::endl;
    }

    void PrintlnWarn(const char* msg) {
        std::cout << SET_COLOR_YELLOW << "[WARN] " << msg << COLOR_RESET << std::endl;
    }

    void PrintlnWarn(int i) {
        std::cout << SET_COLOR_YELLOW << "[WARN] " << i << COLOR_RESET << std::endl;
    }

    void PrintlnCrit(const char* msg) {
        std::cerr << SET_COLOR_RED << "[CRIT] " << msg << COLOR_RESET << std::endl;
    }

    void PrintlnCrit(int i) {
        std::cerr << SET_COLOR_RED << "[CRIT] " << i << COLOR_RESET << std::endl;
    }

} // namespace PrintUtil

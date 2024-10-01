#pragma once

#include <iostream>

namespace PrintUtil {
    void PrintlnInfo(const char* msg);
    void PrintlnWarn(const char* msg);
    void PrintlnCrit(const char* msg);
    void PrintlnInfo(int i);
    void PrintlnWarn(int i);
    void PrintlnCrit(int i);

} // namespace PrintUtil

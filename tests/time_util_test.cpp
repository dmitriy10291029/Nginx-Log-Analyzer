#include <iostream>

#include "lib/utils/time_util.hpp"
#include "test_util.hpp"


void Test1() {
    ASSERT_TRUE(true);
}

void Test2() {
    ASSERT_FALSE(true);
}

int main(int, char**) {
    Test1();
    Test2();

    return 0;
}

#include "test_util.hpp"
#include "lib/log_analyzer/log_analyzer.hpp"

TESTS_BEGIN

TEST("Perfomance test 1: 1 891 715 requests") {
    TIMER_START;

    LogAnalyzer log_analyzer;
    char* argv[] = {nullptr, (char*)"./../../res/NASA_access_log_Jul95"};
    int argc = 2;
    log_analyzer.SetUp(argc, argv);
    int err_code = log_analyzer.Run();
    ASSERT_EQUAL(0, err_code);

    TIMER_STOP;
}

TEST("Perfomance test 2: 10 365 152 requests") {
    TIMER_START;

    LogAnalyzer log_analyzer;
    char* argv[] = {nullptr, (char*)"./../../res/access.log"};
    int argc = 2;
    log_analyzer.SetUp(argc, argv);
    int err_code = log_analyzer.Run();
    ASSERT_EQUAL(0, err_code);

    TIMER_STOP;
}

TESTS_END

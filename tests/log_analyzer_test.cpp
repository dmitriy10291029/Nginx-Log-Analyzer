#include "test_util.hpp"
#include "lib/log_analyzer/log_analyzer.hpp"

TESTS_BEGIN

TEST("Example 1") {
    LogAnalyzer log_analyzer;
    char* argv[] = {nullptr, 
        (char*)"./../../res/example_1.log", 
        (char*)"--from=804571202",
        (char*)"--to=804571260",
        (char*)"-p"
    };
    int argc = 5;
    log_analyzer.SetUp(argc, argv);
    int err_code = log_analyzer.Run();
    ASSERT_EQUAL(0, err_code);
    ASSERT_EQUAL(3, log_analyzer.GetErrorRequests());
    ASSERT_EQUAL(4, log_analyzer.GetTotalRequests());
}

TEST("Perfomance test 1: 1 891 715 requests") {
    TIMER_START;

    LogAnalyzer log_analyzer;
    char* argv[] = {nullptr, (char*)"./../../res/NASA_access_log_Jul95", (char*)"-s"};
    int argc = 3;
    log_analyzer.SetUp(argc, argv);
    int err_code = log_analyzer.Run();
    ASSERT_EQUAL(0, err_code);
    ASSERT_EQUAL(76, log_analyzer.GetErrorRequests());
    ASSERT_EQUAL(1891715, log_analyzer.GetTotalRequests());

    TIMER_STOP;
}

TEST("Perfomance test 2: 10 365 152 requests") {
    TIMER_START

    LogAnalyzer log_analyzer;
    char* argv[] = {nullptr, (char*)"./../../res/access.log", (char*)"-o=./../../res/output.txt", (char*)"-sp"};
    int argc = 4;
    log_analyzer.SetUp(argc, argv);
    int err_code = log_analyzer.Run();
    ASSERT_EQUAL(0, err_code);

    TIMER_STOP
}

TESTS_END

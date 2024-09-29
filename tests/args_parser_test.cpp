#include <iostream>
#include <cstdint>
#include <cstring>

#include "lib/args_parser/args_parser.hpp"
#include "test_util.hpp"

TESTS_BEGIN

TEST("AddFlag, GetFlag") {
    ArgsParser parser;
    parser.AddFlag("name", 'n');
    ASSERT_TRUE(parser.GetFlag("name").has_value());
}

TEST("Parse long flag -foo") {
    ArgsParser parser;
    parser.AddFlag("foo", 'f');
    char* argv[] = {nullptr, (char*)"--foo"};
    int argc = 2;
    ASSERT_EQUAL(0, parser.Parse(argc, argv));
    ASSERT_TRUE(parser.GetFlag("foo").value().declared);
}

TEST("Parse option --foo=10") {
    ArgsParser parser;
    parser.AddOption("foo", 'f');
    char* argv[] = {nullptr, (char*)"--foo=10"};
    int argc = 2;
    ASSERT_EQUAL(0, parser.Parse(argc, argv));
    ASSERT_TRUE(parser.GetOption("foo").has_value());
    auto opt = parser.GetOption("foo").value();
    ASSERT_TRUE(opt.declared);
    ASSERT_TRUE(opt.value_declared);
    ASSERT_TRUE(0 == strcmp("10", opt.value));
}

TEST("Parse option --foo 10") {
    ArgsParser parser;
    parser.AddOption("foo", 'f');
    char* argv[] = {nullptr, (char*)"--foo", (char*)"10"};
    int argc = 3;
    ASSERT_EQUAL(0, parser.Parse(argc, argv));
    ASSERT_TRUE(parser.GetOption("foo").has_value());
    auto opt = parser.GetOption("foo").value();
    ASSERT_TRUE(opt.declared);
    ASSERT_TRUE(opt.value_declared);
    ASSERT_TRUE(0 == strcmp("10", opt.value));
}

TEST("Parse options --foo=10 --bar 20") {
    ArgsParser parser;
    parser.AddOption("foo", 'f');
    parser.AddOption("bar", 'b');
    char* argv[] = {nullptr, (char*)"--foo=10", (char*)"--bar", (char*)"20"};
    int argc = 4;
    ASSERT_EQUAL(0, parser.Parse(argc, argv));

    ASSERT_EQUAL(0, parser.GetFreeArgsSize());

    ASSERT_TRUE(parser.GetOption("foo").has_value());
    auto opt = parser.GetOption("foo").value();
    ASSERT_TRUE(opt.declared);
    ASSERT_TRUE(opt.value_declared);

    ASSERT_TRUE(0 == strcmp("10", opt.value));
    ASSERT_TRUE(parser.GetOption("bar").has_value());
    opt = parser.GetOption("bar").value();
    ASSERT_TRUE(opt.declared);
    ASSERT_TRUE(opt.value_declared);
    ASSERT_TRUE(0 == strcmp("20", opt.value));
}

TEST("Parse short flag -f") {
    ArgsParser parser;
    parser.AddFlag("foo", 'f');
    parser.AddFlag("bar", 'b');
    char* argv[] = {nullptr, (char*)"-f"};
    int argc = 2;
    ASSERT_EQUAL(0, parser.Parse(argc, argv));
    ASSERT_TRUE(parser.GetFlag("foo").value().declared);
    ASSERT_FALSE(parser.GetFlag("bar").value().declared);
}

TEST("Parse short flags -fb") {
    ArgsParser parser;
    parser.AddFlag("foo", 'f');
    parser.AddFlag("bar", 'b');
    char* argv[] = {nullptr, (char*)"-fb"};
    int argc = 2;
    ASSERT_EQUAL(0, parser.Parse(argc, argv));
    ASSERT_TRUE(parser.GetFlag("foo").value().declared);
    ASSERT_TRUE(parser.GetFlag("bar").value().declared);
}

TEST("Parse short flags -f -b") {
    ArgsParser parser;
    parser.AddFlag("foo", 'f');
    parser.AddFlag("bar", 'b');
    char* argv[] = {nullptr, (char*)"-f", (char*)"-b"};
    int argc = 3;
    ASSERT_EQUAL(0, parser.Parse(argc, argv));
    ASSERT_TRUE(parser.GetFlag("foo").value().declared);
    ASSERT_TRUE(parser.GetFlag("bar").value().declared);
}

TEST("Parse short option -f 10") {
    ArgsParser parser;
    parser.AddOption("foo", 'f');
    char* argv[] = {nullptr, (char*)"-f", (char*)"10"};
    int argc = 3;
    ASSERT_EQUAL(0, parser.Parse(argc, argv));
    ASSERT_TRUE(parser.GetOption("foo").has_value());
    auto opt = parser.GetOption("foo").value();
    ASSERT_TRUE(opt.declared);
    ASSERT_TRUE(opt.value_declared);
    ASSERT_TRUE(0 == strcmp("10", opt.value));
}

TEST("GetFreeArgs -f free") {
    ArgsParser parser;
    parser.AddFlag("foo", 'f');
    char* argv[] = {nullptr, (char*)"-f", (char*)"free"};
    int argc = 3;
    ASSERT_EQUAL(0, parser.Parse(argc, argv));
    ASSERT_EQUAL(1, parser.GetFreeArgsSize());
    ASSERT_TRUE(0 == strcmp("free", parser.GetFreeArgs()[0]));
}

TEST("Add AnalyzeLog options") {
    ArgsParser parser;
    parser.AddFlag("help", 'h')
        .AddFlag("print", 'p')
        .AddOption("output", 'o')
        .AddOption("stats", 's')
        .AddOption("window", 'w')
        .AddOption("from", 'f')
        .AddOption("to", 'e');
    
    TEST("AnalyzeLog access.log --output=result.txt") {
        char* argv[] = {
            (char*)"AnalyzeLog", 
            (char*)"access.log", 
            (char*)"--output=result.txt"
        };
        int argc = 3;
        ASSERT_EQUAL(0, parser.Parse(argc, argv));
        ASSERT_EQUAL(1, parser.GetFreeArgsSize());
        ASSERT_EQUAL(0, strcmp("access.log", parser.GetFreeArgs()[0]));
        ASSERT_TRUE(parser.GetOption("output").value().declared);
        ASSERT_EQUAL(0, strcmp("result.txt", parser.GetOption("output").value().value));
    }

    TEST("AnalyzeLog access.log --output=result.txt --print --stats 5") {
        char* argv[] = {
            (char*)"AnalyzeLog", 
            (char*)"access.log", 
            (char*)"--output=result.txt", 
            (char*)"--print", 
            (char*)"--stats", 
            (char*)"5"
        };

        int argc = 6;
        ASSERT_EQUAL(0, parser.Parse(argc, argv));
        ASSERT_EQUAL(1, parser.GetFreeArgsSize());
        ASSERT_EQUAL(0, strcmp("access.log", parser.GetFreeArgs()[0]));
        ASSERT_EQUAL(0, strcmp("result.txt", parser.GetOption("output").value().value));
        ASSERT_TRUE(parser.GetFlag("print").value().declared);
        ASSERT_TRUE(parser.GetOption("stats").value().declared);
        ASSERT_EQUAL(0, strcmp("5", parser.GetOption("stats").value().value));
    }

    TEST("AnalyzeLog --stats=2 --window=60 --from=805821284 --to=807117284 access.log") {
        char* argv[] = {
            (char*)"AnalyzeLog", 
            (char*)"--stats=2", 
            (char*)"--window=60", 
            (char*)"--from=805821284", 
            (char*)"--to=807117284", 
            (char*)"access.log"
        };

        int argc = 6;
        ASSERT_EQUAL(0, parser.Parse(argc, argv));
        ASSERT_EQUAL(1, parser.GetFreeArgsSize());
        ASSERT_EQUAL(0, strcmp("access.log", parser.GetFreeArgs()[0]));

        ASSERT_TRUE(parser.GetOption("stats").value().declared);
        ASSERT_EQUAL(0, strcmp("2", parser.GetOption("stats").value().value));

        ASSERT_TRUE(parser.GetOption("window").value().declared);
        ASSERT_EQUAL(0, strcmp("60", parser.GetOption("window").value().value));

        ASSERT_TRUE(parser.GetOption("from").value().declared);
        ASSERT_EQUAL(0, strcmp("805821284", parser.GetOption("from").value().value));

        ASSERT_TRUE(parser.GetOption("to").value().declared);
        ASSERT_EQUAL(0, strcmp("807117284", parser.GetOption("to").value().value));
    }

    TEST("AnalyzeLog -s 2 access.log") {
        char* argv[] = {
            (char*)"AnalyzeLog", 
            (char*)"-s", 
            (char*)"2", 
            (char*)"access.log"
        };

        int argc = 4;
        ASSERT_EQUAL(0, parser.Parse(argc, argv));
        ASSERT_EQUAL(1, parser.GetFreeArgsSize());
        ASSERT_EQUAL(0, strcmp("access.log", parser.GetFreeArgs()[0]));
        
        ASSERT_TRUE(parser.GetOption("stats").value().declared);
        ASSERT_EQUAL(0, strcmp("2", parser.GetOption("stats").value().value));
    }

    TEST("AnalyzeLog -w 10 access.log") {
        char* argv[] = {
            (char*)"AnalyzeLog", 
            (char*)"-w", 
            (char*)"10", 
            (char*)"access.log"
        };

        int argc = 4;
        ASSERT_EQUAL(0, parser.Parse(argc, argv));
        ASSERT_EQUAL(1, parser.GetFreeArgsSize());
        ASSERT_EQUAL(0, strcmp("access.log", parser.GetFreeArgs()[0]));

        ASSERT_TRUE(parser.GetOption("window").value().declared);
        ASSERT_EQUAL(0, strcmp("10", parser.GetOption("window").value().value));
    }

    TEST("AnalyzeLog access.log -w 10") {
        char* argv[] = {
            (char*)"AnalyzeLog", 
            (char*)"access.log", 
            (char*)"-w", 
            (char*)"10"
        };

        int argc = 4;
        ASSERT_EQUAL(0, parser.Parse(argc, argv));
        ASSERT_EQUAL(1, parser.GetFreeArgsSize());
        ASSERT_EQUAL(0, strcmp("access.log", parser.GetFreeArgs()[0]));

        ASSERT_TRUE(parser.GetOption("window").value().declared);
        ASSERT_EQUAL(0, strcmp("10", parser.GetOption("window").value().value));
    }
}

TESTS_END

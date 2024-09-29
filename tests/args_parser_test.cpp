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
    char* argv[] = {nullptr, (char*)"-f", (char*)"b"};
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

TESTS_END

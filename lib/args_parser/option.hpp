#pragma once

struct Option {
    char short_name;
    const char* long_name;
    bool declared = false;
    bool value_declared = false;
    const char* value;
};

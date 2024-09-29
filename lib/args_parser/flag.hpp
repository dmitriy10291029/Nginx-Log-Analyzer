#pragma once

struct Flag {
    char short_name;
    const char* long_name;
    bool declared = false;
};

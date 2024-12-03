#pragma once
#include <string>
#include <string.h> // strerror
#include <cstdio> // fprintf
#include <cstdlib> // exit

template <typename... Args> /* This mimics GNU 'error' function */
inline void error(int status, int errnum, const char *format, Args... args) {
    fprintf(stderr, (format + std::string(errnum ? ": %s\n" : "\n")).c_str(), args..., strerror(errnum));
    if (status) exit(status);
}
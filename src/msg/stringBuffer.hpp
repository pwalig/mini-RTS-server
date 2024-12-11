#pragma once

#include <vector>
#include <string>

namespace message {
    void appendString(std::vector<char>& buff, const std::string& s);
    void appendStringWDelim(std::vector<char>& buff, const std::string& s, char delim = '\0');

    template <typename T>
    inline void appendNumber(std::vector<char>& buff, T number) {
        appendString(buff, std::to_string(number));
    }
    template <typename T>
    inline void appendNumberWDelim(std::vector<char>& buff, T number, char delim = '\0') {
        appendStringWDelim(buff, std::to_string(number), delim);
    }
}
#include "stringBuffer.h"
#include <memory.h>

void message::appendString(std::vector<char>& buff, const std::string& s) {
    unsigned long it = buff.size();
    buff.resize(buff.size() + s.size());
    memcpy(&buff.data()[it], (void*)s.c_str(), s.size());
}

void message::appendStringWDelim(std::vector<char>& buff, const std::string& s, char delim) {
    appendString(buff, s);
    buff.push_back(delim);
}
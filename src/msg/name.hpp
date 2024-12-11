#pragma once

#include <string>

#include <msg/base.hpp>

namespace message {
    class name : public base {
    public:
        std::string _name;
        name(const std::string& name_);
    };
}
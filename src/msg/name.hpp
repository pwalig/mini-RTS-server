#pragma once

#include <string>

#include <msg/base.hpp>

namespace message {
    class name : public base {
    public:
        const std::string _name;
        name(const std::string& name_) : base(), _name(name_) {}
    };
}
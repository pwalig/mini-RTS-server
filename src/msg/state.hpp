#pragma once

#include <msg/base.hpp>

namespace message {
    class state : public base
    {
    private:
    public:
        enum class action {
            disconnect, joinRequest, quit
        };

        const action act;

        state(action a) : base(), act(a){}
    };
        
}

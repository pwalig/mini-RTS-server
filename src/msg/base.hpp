#pragma once

namespace message
{
    enum class type {
        name, move, attack, mine, getBoard, getPlayers, invalid, disconnect, joinRequest, quit
    };

    class base {
        public:
        const type typ;
        base(const type& tp);
        virtual ~base() = default;
    };
}
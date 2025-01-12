#pragma once

#include <msg/addressUnit.hpp>

namespace rts {
    class field;
}

namespace message {
    class command : public base {
        addressUnit* source;
        public:
        rts::unit* getUnit(rts::game* game) const;

        command(unsigned int sourceId);
        command(unsigned int sourceX, unsigned int sourceY);

        ~command();
    };

    class move : public command {
        const unsigned int destX;
        const unsigned int destY;

        public:
        rts::field* getDestinationField(rts::game* game) const;

        move(
            unsigned int sx, unsigned int sy,
            unsigned int dx, unsigned int dy
        );

        move(
            unsigned int si,
            unsigned int dx, unsigned int dy
        );
    };
    
    class attack : public command {
        addressUnit* target;

        public:
        rts::unit* getTarget(rts::game* game) const;

        attack(
            unsigned int sx, unsigned int sy,
            unsigned int tx, unsigned int ty
        );
            
        attack(
            unsigned int si,
            unsigned int ti
        );

        ~attack();
    };
    
    class mine : public command {
        public:

        mine(
            unsigned int sx, unsigned int sy
        );

        mine(
            unsigned int si
        );
    };
}
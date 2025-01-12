#pragma once

#include <msg/base.hpp>

namespace rts {
    class unit;
    class game;
}

namespace message {

    class addressUnit {
    public:
        virtual rts::unit* getUnit(const rts::game* g) const = 0;
        virtual ~addressUnit() = default;
    };

    class addressUnitByCoordinates : public addressUnit {
    public:
        const unsigned int x, y;

        addressUnitByCoordinates(unsigned int xpos, unsigned int ypos);
        rts::unit* getUnit(const rts::game* g) const override;
    };

    class addressUnitById : public addressUnit {
    public:
        const unsigned int unitId;
        
        addressUnitById(unsigned int id);
        rts::unit* getUnit(const rts::game* g) const override;
    };
}
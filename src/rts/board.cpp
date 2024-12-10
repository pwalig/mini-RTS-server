#include "board.h"

#include <cassert>
#include "unit.h"

rts::board::board() : gen(std::random_device()()) {
    for (unsigned int x = 0; x < 256; ++x) {
        fields.push_back(std::vector<field>());
        for (unsigned int y = 0; y < 256; ++y) {
            fields[x].push_back(field(x, y));
        }
    }
}

std::vector<char> rts::board::boardStateMessage() const {
    std::vector<char> data;
    return data;
}
rts::field& rts::board::getField(const unsigned int& xpos, const unsigned int& ypos) {
    return fields[xpos][ypos];
}

std::vector<rts::field*> rts::board::resourceFields(bool resource) {
    std::vector<field*> out;
    for (std::vector<field>& row : fields){
        for (field& f : row) {
            if (f.hasResource() == resource) out.push_back(&f);
        }
    }
    return out;
}

std::vector<rts::field*> rts::board::emptyFields(bool empty) {
    std::vector<field*> out;
    for (std::vector<field>& row : fields){
        for (field& f : row) {
            if (f.empty() == empty) out.push_back(&f);
        }
    }
    return out;
}

rts::field* rts::board::randomField() {
    std::uniform_int_distribution<> distrib(0, 255);
    return &fields[distrib(gen)][distrib(gen)];
}
rts::field* rts::board::randomEmptyField(bool empty) {
    std::vector<field*> resFields = emptyFields(empty);
    std::uniform_int_distribution<> distrib(0, resFields.size() - 1);
    return resFields[distrib(gen)];
}
rts::field* rts::board::randomResourceField(bool resource) {
    std::vector<field*> resFields = resourceFields(resource);
    std::uniform_int_distribution<> distrib(0, resFields.size() - 1);
    return resFields[distrib(gen)];
}

void rts::board::spawnResource() {
    randomField()->spawnResource();
}

void rts::board::spawnResources(const unsigned int& amount) {
    for (unsigned int i = 0; i < amount; ++i) {
        spawnResource();
    }
}
#include <rts/board.hpp>

#include <cassert>
#include <rts/unit.hpp>

rts::board::board(unsigned int X, unsigned int Y) : gen(std::random_device()()) {
    for (unsigned int x = 0; x < X; ++x) {
        fields.push_back(std::vector<field>());
        for (unsigned int y = 0; y < Y; ++y) {
            fields[x].push_back(field(x, y));
        }
    }
}

rts::field* rts::board::getField(const unsigned int& xpos, const unsigned int& ypos) {
    if (xpos < fields.size() && ypos < fields[0].size())
        return &fields[xpos][ypos];
    else return nullptr;
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
    std::uniform_int_distribution<> distrib(0, fields.size() - 1);
    return &fields[distrib(gen)][distrib(gen)];
}
rts::field* rts::board::randomEmptyField(bool empty) {
    std::vector<field*> resFields = emptyFields(empty);
    if (resFields.empty()) return nullptr;
    std::uniform_int_distribution<> distrib(0, resFields.size() - 1);
    return resFields[distrib(gen)];
}
rts::field* rts::board::randomResourceField(bool resource) {
    std::vector<field*> resFields = resourceFields(resource);
    if (resFields.empty()) return nullptr;
    std::uniform_int_distribution<> distrib(0, resFields.size() - 1);
    return resFields[distrib(gen)];
}
rts::field* rts::board::closestEmptyField(const field* source) {
    unsigned int minDist = std::numeric_limits<unsigned int>::max();
    field* candidate = nullptr;
    std::vector<field*> available = emptyFields(true);
    for (field* f : available) {
        unsigned int dist = source->distance(*f);
        if (dist < minDist) {
            minDist = dist;
            candidate = f;
        }
    }
    return candidate;
}

void rts::board::spawnResource(unsigned int hp) {
    randomResourceField(false)->spawnResource(hp);
}

void rts::board::spawnResources(unsigned int amount, unsigned int hp) {
    for (unsigned int i = 0; i < amount; ++i) {
        spawnResource(hp);
    }
}
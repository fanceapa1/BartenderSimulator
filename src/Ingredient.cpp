#include "Ingredient.h"

#include <utility>

Ingredient::Ingredient(std::string name, int volumeMl)
    : name(std::move(name)), volumeMl(volumeMl) {
}

const std::string& Ingredient::getName() const {
    return name;
}

int Ingredient::getVolumeMl() const {
    return volumeMl;
}

Alcohol::Alcohol(std::string name, int volumeMl, double abv)
    : Ingredient(std::move(name), volumeMl), abv(abv) {
}

double Alcohol::getABV() const {
    return abv;
}

Mixer::Mixer(std::string name, int volumeMl, double sweetness)
    : Ingredient(std::move(name), volumeMl), sweetness(sweetness) {
}

double Mixer::getSweetness() const {
    return sweetness;
}

double Mixer::getABV() const {
    return 0;
}

Garnish::Garnish(std::string name, int volumeMl)
    : Ingredient(std::move(name), volumeMl) {
}

double Garnish::getABV() const {
    return 0;
}

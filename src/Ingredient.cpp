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

void Ingredient::addVolumeMl(int extraVolumeMl) {
    volumeMl += extraVolumeMl;
}

Alcohol::Alcohol(std::string name, int volumeMl, double abv)
    : Ingredient(std::move(name), volumeMl), abv(abv) {
}

double Alcohol::getABV() const {
    return abv;
}

Ingredient* Alcohol::clone() const {
    return new Alcohol(*this);
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

Ingredient* Mixer::clone() const {
    return new Mixer(*this);
}

Garnish::Garnish(std::string name, int volumeMl)
    : Ingredient(std::move(name), volumeMl) {
}

double Garnish::getABV() const {
    return 0;
}

Ingredient* Garnish::clone() const {
    return new Garnish(*this);
}

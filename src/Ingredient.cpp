#include "Ingredient.h"

#include <utility>

Ingredient::Ingredient(std::string name, int volumeMl, double basePrice)
    : name(std::move(name)), volumeMl(volumeMl), basePrice(basePrice) {
}

std::string Ingredient::getName() const {
    return name;
}

int Ingredient::getVolumeMl() const {
    return volumeMl;
}

double Ingredient::getBasePrice() const {
    return basePrice;
}

Alcohol::Alcohol(std::string name, int volumeMl, double basePrice, double abv)
    : Ingredient(std::move(name), volumeMl, basePrice), abv(abv) {
}

double Alcohol::getABV() const {
    return abv;
}

Mixer::Mixer(std::string name, int volumeMl, double basePrice, double sweetness)
    : Ingredient(std::move(name), volumeMl, basePrice), sweetness(sweetness) {
}

double Mixer::getSweetness() const {
    return sweetness;
}

double Mixer::getABV() const {
    return 0;
}

Garnish::Garnish(std::string name, int volumeMl, double basePrice)
    : Ingredient(std::move(name), volumeMl, basePrice) {
}

double Garnish::getABV() const {
    return 0;
}

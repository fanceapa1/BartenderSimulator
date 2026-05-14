#ifndef BARTENDER_DRINK_H
#define BARTENDER_DRINK_H

#include "Ingredient.h"

#include <cstddef>
#include <iosfwd>
#include <string>
#include <utility>
#include <vector>

class Drink {
protected:
    std::string name;
    double capacityMl;

public:
    virtual ~Drink() = default;

    // TODO: add constructors and shared drink behavior.
};

class Recipe : public Drink {
private:
    std::vector<std::pair<std::string, double>> ingredientRatios;
    double menuPrice;

public:
    // TODO: implement recipe input/output once the file format is decided.
    friend std::ostream& operator<<(std::ostream& out, const Recipe& recipe);
    friend std::istream& operator>>(std::istream& in, Recipe& recipe);
};

class Concoction : public Drink {
private:
    Ingredient** ingredients;
    std::size_t ingredientCount;
    std::size_t ingredientCapacity;

public:
    // TODO: implement Rule of Three/Five here.
    // TODO: add pour(), serve-related helpers, and operator+= later.
};

bool operator==(const Concoction& concoction, const Recipe& recipe);
bool operator==(const Recipe& recipe, const Concoction& concoction);

#endif

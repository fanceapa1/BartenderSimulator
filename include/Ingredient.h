#ifndef BARTENDER_INGREDIENT_H
#define BARTENDER_INGREDIENT_H

#include <string>

class Ingredient {
protected:
    std::string name;
    double volumeMl;
    double basePrice;

public:
    virtual ~Ingredient() = default;

    // TODO: add constructors, getters, and common ingredient behavior.
    virtual double getABV() const = 0;
};

class Alcohol : public Ingredient {
private:
    double abv;
};

class Mixer : public Ingredient {
private:
    double sweetness;
};

class Garnish : public Ingredient {
private:
    bool chilled;
};

#endif

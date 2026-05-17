#ifndef BARTENDER_INGREDIENT_H
#define BARTENDER_INGREDIENT_H

#include <string>

class Ingredient {
protected:
    std::string name;
    int volumeMl;
    double basePrice;

public:
    Ingredient(std::string name, int volumeMl, double basePrice);
    virtual ~Ingredient() = default;

    [[nodiscard]] std::string getName() const;
    [[nodiscard]] int getVolumeMl() const;
    [[nodiscard]] double getBasePrice() const;
    [[nodiscard]] virtual double getABV() const = 0;
};

class Alcohol : public Ingredient {
private:
    double abv;

public:
    Alcohol(std::string name, int volumeMl, double basePrice, double abv);
    [[nodiscard]] double getABV() const override;
};

class Mixer : public Ingredient {
private:
    double sweetness;

public:
    Mixer(std::string name, int volumeMl, double basePrice, double sweetness);

    [[nodiscard]] double getSweetness() const;
    [[nodiscard]] double getABV() const override;
};

class Garnish : public Ingredient {
public:
    Garnish(std::string name, int volumeMl, double basePrice);

    [[nodiscard]] double getABV() const override;
};

#endif

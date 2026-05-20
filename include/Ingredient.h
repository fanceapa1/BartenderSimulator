#ifndef BARTENDER_INGREDIENT_H
#define BARTENDER_INGREDIENT_H

#include <string>

class Ingredient {
protected:
    std::string name;
    int volumeMl;

public:
    Ingredient(std::string name, int volumeMl);
    virtual ~Ingredient() = default;

    [[nodiscard]] const std::string& getName() const;
    [[nodiscard]] int getVolumeMl() const;
    [[nodiscard]] virtual double getABV() const = 0;
};

class Alcohol : public Ingredient {
private:
    double abv;

public:
    Alcohol(std::string name, int volumeMl, double abv);
    [[nodiscard]] double getABV() const override;
};

class Mixer : public Ingredient {
private:
    double sweetness;

public:
    Mixer(std::string name, int volumeMl, double sweetness);

    [[nodiscard]] double getSweetness() const;
    [[nodiscard]] double getABV() const override;
};

class Garnish : public Ingredient {
public:
    Garnish(std::string name, int volumeMl);

    [[nodiscard]] double getABV() const override;
};

#endif

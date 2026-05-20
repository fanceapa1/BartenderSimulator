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
    Drink();
    Drink(std::string name, double capacityMl);
    virtual ~Drink() = default;

    [[nodiscard]] const std::string& getName() const;
    [[nodiscard]] double getCapacityMl() const;
};

class Recipe : public Drink {
private:
    std::vector<std::pair<std::string, double>> ingredientRatios;
    double menuPrice;

public:
    Recipe();
    Recipe(std::string name, double menuPrice, double capacityMl);
    Recipe(std::string name,
           double menuPrice,
           double capacityMl,
           std::vector<std::pair<std::string, double>> ingredientRatios);

    [[nodiscard]] double getMenuPrice() const;
    [[nodiscard]] const std::vector<std::pair<std::string, double>>& getIngredientRatios() const;

    friend std::ostream& operator<<(std::ostream& out, const Recipe& recipe);
    friend std::istream& operator>>(std::istream& in, Recipe& recipe);

    [[nodiscard]] double getABV() const;
    [[nodiscard]] double getSweetness() const;
};

class Concoction : public Drink {
private:
    Ingredient** ingredients;
    std::size_t ingredientCount;
    std::size_t ingredientCapacity;

    void clearIngredients();
    void reserveIngredients(std::size_t newCapacity);

public:
    Concoction();
    Concoction(std::string name, double capacityMl);
    Concoction(const Concoction& other) = delete;
    Concoction& operator=(const Concoction& other) = delete;
    ~Concoction() override;

    [[nodiscard]] std::size_t getIngredientCount() const;
    [[nodiscard]] std::size_t getIngredientCapacity() const;
    [[nodiscard]] double getTotalVolume() const;
    [[nodiscard]] double getABV() const;
    [[nodiscard]] double getSweetness() const;
    [[nodiscard]] bool containsIngredient(const std::string& ingredientName) const;
    [[nodiscard]] static bool isKnownIngredient(const std::string& ingredientName);

    void pour(const std::string& ingredientName, int amount);
    void printIngredients(std::ostream& out) const;
    void reset();
};

bool operator==(const Concoction& concoction, const Recipe& recipe);
bool operator==(const Recipe& recipe, const Concoction& concoction);

#endif

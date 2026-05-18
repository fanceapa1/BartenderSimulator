#include "Drink.h"

#include <istream>
#include <ostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

namespace {
std::string trim(const std::string& text) {
    const std::string whitespace = " \t\r\n";
    const std::size_t first = text.find_first_not_of(whitespace);

    if(first == std::string::npos) {
        return "";
    }

    const std::size_t last = text.find_last_not_of(whitespace);
    return text.substr(first, last - first + 1);
}

double convertRecipeAmountToMl(const std::string& ingredientName, double amount) {
    if(ingredientName == "Ice") {
        return amount * 25;
    }

    if(ingredientName == "Lemon") {
        return amount * 10;
    }

    return amount;
}

double getKnownIngredientABV(const std::string& ingredientName) {
    if(ingredientName == "Gin") {
        return 0.40;
    }

    if(ingredientName == "Vermouth") {
        return 0.15;
    }

    return 0;
}
}

Drink::Drink()
    : name("Unnamed drink"), capacityMl(250) {
}

Drink::Drink(std::string name, double capacityMl)
    : name(std::move(name)), capacityMl(capacityMl) {
}

const std::string& Drink::getName() const {
    return name;
}

double Drink::getCapacityMl() const {
    return capacityMl;
}

Recipe::Recipe()
    : Drink("Unnamed recipe", 250), ingredientRatios(), menuPrice(0) {
}

Recipe::Recipe(std::string name, double menuPrice, double capacityMl)
    : Drink(std::move(name), capacityMl), ingredientRatios(), menuPrice(menuPrice) {
}

Recipe::Recipe(std::string name,
               double menuPrice,
               double capacityMl,
               std::vector<std::pair<std::string, double>> ingredientRatios)
    : Drink(std::move(name), capacityMl),
      ingredientRatios(std::move(ingredientRatios)),
      menuPrice(menuPrice) {
}

double Recipe::getMenuPrice() const {
    return menuPrice;
}

const std::vector<std::pair<std::string, double>>& Recipe::getIngredientRatios() const {
    return ingredientRatios;
}

double Recipe::getABV() const {
    double totalABV = 0;

    for(const auto& ingredientRatio : ingredientRatios) {
        const std::string& ingredientName = ingredientRatio.first;
        const double ratio = ingredientRatio.second;
        totalABV += getKnownIngredientABV(ingredientName) * ratio;
    }

    return totalABV;
}

std::ostream& operator<<(std::ostream& out, const Recipe& recipe) {
    out << "Recipe: " << recipe.name << "\n";
    out << "Total volume: " << recipe.capacityMl << " ml\n";
    out << "Menu price: " << recipe.menuPrice << "\n";
    out << "ABV: " << recipe.getABV() * 100 << "%\n";
    out << "Ingredients:\n";

    for(const auto& ingredientRatio : recipe.ingredientRatios) {
        const double convertedMl = ingredientRatio.second * recipe.capacityMl;
        out << "- " << ingredientRatio.first << ": "
            << convertedMl << " ml, ratio "
            << ingredientRatio.second << "\n";
    }

    return out;
}

std::istream& operator>>(std::istream& in, Recipe& recipe) {
    std::string recipeName;
    if(!std::getline(in >> std::ws, recipeName)) {
        return in;
    }

    std::vector<std::pair<std::string, double>> ingredientVolumes;
    double totalVolume = 0;

    while(true) {
        std::string ingredientName;
        if(!std::getline(in, ingredientName)) {
            if(in.eof()) {
                in.clear(in.rdstate() & ~std::ios::failbit);
            }
            break;
        }

        ingredientName = trim(ingredientName);

        if(ingredientName.empty()) {
            continue;
        }

        std::string amountLine;
        if(!std::getline(in, amountLine)) {
            in.setstate(std::ios::failbit);
            return in;
        }

        std::istringstream amountInput(amountLine);
        double amount = 0;
        if(!(amountInput >> amount)) {
            in.setstate(std::ios::failbit);
            return in;
        }

        const double convertedVolume = convertRecipeAmountToMl(ingredientName, amount);
        ingredientVolumes.emplace_back(ingredientName, convertedVolume);
        totalVolume += convertedVolume;
    }

    recipe.name = recipeName;
    recipe.menuPrice = 0;
    recipe.capacityMl = totalVolume;
    recipe.ingredientRatios.clear();

    if(totalVolume > 0) {
        for(const auto& ingredientVolume : ingredientVolumes) {
            recipe.ingredientRatios.emplace_back(ingredientVolume.first, ingredientVolume.second / totalVolume);
        }
    }

    return in;
}

Concoction::Concoction()
    : Drink("Player concoction", 250), ingredients(nullptr), ingredientCount(0), ingredientCapacity(0) {
}

Concoction::Concoction(std::string name, double capacityMl)
    : Drink(std::move(name), capacityMl), ingredients(nullptr), ingredientCount(0), ingredientCapacity(0) {
}

std::size_t Concoction::getIngredientCount() const {
    return ingredientCount;
}

std::size_t Concoction::getIngredientCapacity() const {
    return ingredientCapacity;
}

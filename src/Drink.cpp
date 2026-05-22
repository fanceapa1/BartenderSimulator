#include "Drink.h"

#include "Exceptions.h"

#include <cctype>
#include <istream>
#include <iostream>
#include <memory>
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

std::string normalizedName(const std::string& text) {
    std::string normalized = trim(text);

    for(char& character : normalized) {
        character = static_cast<char>(std::tolower(static_cast<unsigned char>(character)));
    }

    return normalized;
}

double convertRecipeAmountToMl(const std::string& ingredientName, double amount) {
    const std::string normalizedIngredientName = normalizedName(ingredientName);

    if(normalizedIngredientName == "ice") {
        return amount * 25;
    }

    if(normalizedIngredientName == "lemon") {
        return amount * 10;
    }

    if(normalizedIngredientName == "lime") {
        return amount * 10;
    }

    if(normalizedIngredientName == "grapefruit") {
        return amount * 15;
    }

    if(normalizedIngredientName == "orange") {
        return amount * 15;
    }

    return amount;
}

double getKnownIngredientABV(const std::string& ingredientName) {
    const std::string normalizedIngredientName = normalizedName(ingredientName);

    if(normalizedIngredientName == "gin" ||
       normalizedIngredientName == "vodka" ||
       normalizedIngredientName == "whiskey" ||
       normalizedIngredientName == "rum" ||
       normalizedIngredientName == "tequila") {
        return 0.40;
    }

    if(normalizedIngredientName == "vermouth") {
        return 0.15;
    }

    if(normalizedIngredientName == "aperol") {
        return 0.11;
    }

    if(normalizedIngredientName == "sparkling wine") {
        return 0.12;
    }

    return 0;
}

double getKnownIngredientSweetness(const std::string& ingredientName) {
    const std::string normalizedIngredientName = normalizedName(ingredientName);

    if(normalizedIngredientName == "tonic") {
        return 0.20;
    }

    if(normalizedIngredientName == "soda") {
        return 0.05;
    }

    if(normalizedIngredientName == "orange juice") {
        return 0.80;
    }

    if(normalizedIngredientName == "cola") {
        return 0.70;
    }

    if(normalizedIngredientName == "ginger beer") {
        return 0.60;
    }

    if(normalizedIngredientName == "simple syrup" || normalizedIngredientName == "grenadine") {
        return 1.00;
    }

    return 0;
}

bool isKnownIngredientName(const std::string& ingredientName) {
    const std::string normalizedIngredientName = normalizedName(ingredientName);

    return normalizedIngredientName == "gin" ||
           normalizedIngredientName == "vermouth" ||
           normalizedIngredientName == "vodka" ||
           normalizedIngredientName == "whiskey" ||
           normalizedIngredientName == "rum" ||
           normalizedIngredientName == "tequila" ||
           normalizedIngredientName == "aperol" ||
           normalizedIngredientName == "sparkling wine" ||
           normalizedIngredientName == "tonic" ||
           normalizedIngredientName == "soda" ||
           normalizedIngredientName == "orange juice" ||
           normalizedIngredientName == "cola" ||
           normalizedIngredientName == "ginger beer" ||
           normalizedIngredientName == "simple syrup" ||
           normalizedIngredientName == "grenadine" ||
           normalizedIngredientName == "ice" ||
           normalizedIngredientName == "lemon" ||
           normalizedIngredientName == "lime" ||
           normalizedIngredientName == "grapefruit" ||
           normalizedIngredientName == "orange";
}

Ingredient* createKnownPouredIngredient(const std::string& ingredientName, int amount) {
    const std::string normalizedIngredientName = normalizedName(ingredientName);

    if(normalizedIngredientName == "gin") {
        return new Alcohol("Gin", amount, 0.40);
    }

    if(normalizedIngredientName == "vermouth") {
        return new Alcohol("Vermouth", amount, 0.15);
    }

    if(normalizedIngredientName == "vodka") {
        return new Alcohol("Vodka", amount, 0.40);
    }

    if(normalizedIngredientName == "whiskey") {
        return new Alcohol("Whiskey", amount, 0.40);
    }

    if(normalizedIngredientName == "rum") {
        return new Alcohol("Rum", amount, 0.40);
    }

    if(normalizedIngredientName == "tequila") {
        return new Alcohol("Tequila", amount, 0.40);
    }

    if(normalizedIngredientName == "aperol") {
        return new Alcohol("Aperol", amount, 0.11);
    }

    if(normalizedIngredientName == "sparkling wine") {
        return new Alcohol("Sparkling Wine", amount, 0.12);
    }

    if(normalizedIngredientName == "tonic") {
        return new Mixer("Tonic", amount, 0.20);
    }

    if(normalizedIngredientName == "soda") {
        return new Mixer("Soda", amount, 0.05);
    }

    if(normalizedIngredientName == "orange juice") {
        return new Mixer("Orange Juice", amount, 0.80);
    }

    if(normalizedIngredientName == "cola") {
        return new Mixer("Cola", amount, 0.70);
    }

    if(normalizedIngredientName == "ginger beer") {
        return new Mixer("Ginger Beer", amount, 0.60);
    }

    if(normalizedIngredientName == "simple syrup") {
        return new Mixer("Simple Syrup", amount, 1.00);
    }

    if(normalizedIngredientName == "grenadine") {
        return new Mixer("Grenadine", amount, 1.00);
    }

    if(normalizedIngredientName == "ice") {
        return new Garnish("Ice", amount * 25);
    }

    if(normalizedIngredientName == "lemon") {
        return new Garnish("Lemon", amount * 10);
    }

    if(normalizedIngredientName == "lime") {
        return new Garnish("Lime", amount * 10);
    }

    if(normalizedIngredientName == "grapefruit") {
        return new Garnish("Grapefruit", amount * 15);
    }

    if(normalizedIngredientName == "orange") {
        return new Garnish("Orange", amount * 15);
    }

    return nullptr;
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

double Recipe::getSweetness() const {
    double totalSweetness = 0;

    for(const auto& ingredientRatio : ingredientRatios) {
        const std::string& ingredientName = ingredientRatio.first;
        const double ratio = ingredientRatio.second;
        totalSweetness += getKnownIngredientSweetness(ingredientName) * ratio;
    }

    return totalSweetness;
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
    : Drink("Player concoction", 500), ingredients(nullptr), ingredientCount(0), ingredientCapacity(0) {
}

Concoction::Concoction(std::string name, double capacityMl)
    : Drink(std::move(name), capacityMl), ingredients(nullptr), ingredientCount(0), ingredientCapacity(0) {
}

Concoction::Concoction(const Concoction& other)
    : Drink(other.name, other.capacityMl), ingredients(nullptr), ingredientCount(0), ingredientCapacity(0) {
    try {
        reserveIngredients(other.ingredientCapacity);

        for(std::size_t index = 0; index < other.ingredientCount; ++index) {
            ingredients[ingredientCount] = other.ingredients[index]->clone();
            ++ingredientCount;
        }
    } catch(...) {
        clearIngredients();
        throw;
    }
}

Concoction& Concoction::operator=(const Concoction& other) {
    if(this != &other) {
        Concoction copy(other);

        std::swap(name, copy.name);
        std::swap(capacityMl, copy.capacityMl);
        std::swap(ingredients, copy.ingredients);
        std::swap(ingredientCount, copy.ingredientCount);
        std::swap(ingredientCapacity, copy.ingredientCapacity);
    }

    return *this;
}

Concoction::~Concoction() {
    clearIngredients();
}

std::size_t Concoction::getIngredientCount() const {
    return ingredientCount;
}

std::size_t Concoction::getIngredientCapacity() const {
    return ingredientCapacity;
}

double Concoction::getTotalVolume() const {
    double totalVolume = 0;

    for(std::size_t index = 0; index < ingredientCount; ++index) {
        totalVolume += ingredients[index]->getVolumeMl();
    }

    return totalVolume;
}

double Concoction::getABV() const {
    const double totalVolume = getTotalVolume();

    if(totalVolume == 0) {
        return 0;
    }

    double alcoholVolume = 0;

    for(std::size_t index = 0; index < ingredientCount; ++index) {
        alcoholVolume += ingredients[index]->getABV() * ingredients[index]->getVolumeMl();
    }

    return alcoholVolume / totalVolume;
}

double Concoction::getSweetness() const {
    const double totalVolume = getTotalVolume();

    if(totalVolume == 0) {
        return 0;
    }

    double sweetness = 0;

    for(std::size_t index = 0; index < ingredientCount; ++index) {
        const auto* mixer = dynamic_cast<const Mixer*>(ingredients[index]);

        if(mixer != nullptr) {
            sweetness += mixer->getSweetness() * mixer->getVolumeMl();
        }
    }

    return sweetness / totalVolume;
}

bool Concoction::hasAlcohol() const {
    for(std::size_t index = 0; index < ingredientCount; ++index) {
        const auto* alcohol = dynamic_cast<const Alcohol*>(ingredients[index]);

        if(alcohol != nullptr) {
            return true;
        }
    }

    return false;
}

bool Concoction::containsIngredient(const std::string& ingredientName) const {
    const std::string normalizedIngredientName = normalizedName(ingredientName);

    for(std::size_t index = 0; index < ingredientCount; ++index) {
        if(normalizedName(ingredients[index]->getName()) == normalizedIngredientName) {
            return true;
        }
    }

    return false;
}

bool Concoction::isKnownIngredient(const std::string& ingredientName) {
    return isKnownIngredientName(ingredientName);
}

Ingredient* Concoction::createPouredIngredient(const std::string& ingredientName, int amount) {
    return createKnownPouredIngredient(ingredientName, amount);
}

Concoction& Concoction::operator+=(const Ingredient& ingredient) {
    if(ingredient.getVolumeMl() <= 0) {
        std::cout << "Invalid amount. Nothing was poured.\n";
        return *this;
    }

    if(getTotalVolume() + ingredient.getVolumeMl() > getCapacityMl()) {
        throw GlassOverflowException();
    }

    const std::string normalizedIngredientName = normalizedName(ingredient.getName());

    for(std::size_t index = 0; index < ingredientCount; ++index) {
        if(normalizedName(ingredients[index]->getName()) == normalizedIngredientName) {
            ingredients[index]->addVolumeMl(ingredient.getVolumeMl());
            std::cout << "Poured " << ingredients[index]->getName() << ".\n";
            return *this;
        }
    }

    std::unique_ptr<Ingredient> ingredientCopy(ingredient.clone());

    if(getIngredientCount() == getIngredientCapacity()) {
        const std::size_t nextCapacity = getIngredientCapacity() == 0 ? 4 : getIngredientCapacity() * 2;
        reserveIngredients(nextCapacity);
    }

    ingredients[getIngredientCount()] = ingredientCopy.release();
    ++ingredientCount;

    std::cout << "Poured " << ingredients[getIngredientCount() - 1]->getName() << ".\n";
    return *this;
}

void Concoction::printIngredients(std::ostream& out) const {
    if(ingredientCount == 0) {
        return;
    }

    out << "Glass: ";

    for(std::size_t index = 0; index < ingredientCount; ++index) {
        if(index != 0) {
            out << ", ";
        }

        out << ingredients[index]->getName() << " " << ingredients[index]->getVolumeMl() << "ml";
    }

    out << "\n";
}

void Concoction::reset() {
    clearIngredients();
}

void Concoction::clearIngredients() {
    for(std::size_t index = 0; index < ingredientCount; ++index) {
        delete ingredients[index];
    }

    delete[] ingredients;
    ingredients = nullptr;
    ingredientCount = 0;
    ingredientCapacity = 0;
}

void Concoction::reserveIngredients(std::size_t newCapacity) {
    if(newCapacity <= ingredientCapacity) {
        return;
    }

    auto** newIngredients = new Ingredient*[newCapacity] {};

    for(std::size_t index = 0; index < ingredientCount; ++index) {
        newIngredients[index] = ingredients[index];
    }

    delete[] ingredients;
    ingredients = newIngredients;
    ingredientCapacity = newCapacity;
}

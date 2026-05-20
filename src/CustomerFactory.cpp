#include "CustomerFactory.h"

#include <memory>
#include <random>
#include <ranges>
#include <string>
#include <utility>
#include <vector>

namespace {
Recipe makeRecipeFromVolumes(const std::string& name,
                             const std::vector<std::pair<std::string, double>>& ingredientVolumes) {
    double totalVolume = 0;

    for(const auto& val : ingredientVolumes | std::views::values) {
        totalVolume += val;
    }

    std::vector<std::pair<std::string, double>> ingredientRatios;

    for(const auto& ingredientVolume : ingredientVolumes) {
        ingredientRatios.emplace_back(ingredientVolume.first, ingredientVolume.second / totalVolume);
    }

    return {name, 0, totalVolume, ingredientRatios};
}
}

std::unique_ptr<Customer> CustomerFactory::createCustomer(const std::string& name, const int typeSeed) {
    const int customerType = ((typeSeed % 3) + 3) % 3;

    if(customerType == 1) {
        return std::make_unique<HeavyDrinker>(name, createRandomDrinkRequest());
    }

    if(customerType == 2) {
        return std::make_unique<Critic>(name, createRandomDrinkRequest());
    }

    return std::make_unique<CasualPatron>(name, createRandomDrinkRequest());
}

Recipe CustomerFactory::createRandomDrinkRequest() {
    static std::random_device randomDevice;
    static std::mt19937 generator(randomDevice());
    static std::uniform_int_distribution<int> drinkDistribution(0, 1);

    if(drinkDistribution(generator) == 0) {
        return makeRecipeFromVolumes("Gin Tonic", {
            {"Gin", 50},
            {"Ice", 3 * 25},
            {"Tonic", 200},
            {"Lemon", 10},
        });
    }

    return makeRecipeFromVolumes("Vermouth Spritz", {
        {"Vermouth", 100},
        {"Ice", 3 * 25},
        {"Soda", 200},
        {"Grapefruit", 15},
    });
}

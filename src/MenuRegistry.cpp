#include "MenuRegistry.h"

#include <algorithm>
#include <cstddef>
#include <random>
#include <utility>

MenuRegistry::MenuRegistry() {
    recipes.push_back(makeRecipeFromVolumes("Gin Tonic", 8, {
        {"Gin", 50},
        {"Ice", 3 * 25},
        {"Tonic", 200},
        {"Lemon", 10},
    }));

    recipes.push_back(makeRecipeFromVolumes("Vermouth Spritz", 10, {
        {"Vermouth", 100},
        {"Ice", 3 * 25},
        {"Soda", 200},
        {"Grapefruit", 15},
    }));
}

Recipe MenuRegistry::makeRecipeFromVolumes(
    const std::string& name,
    double menuPrice,
    const std::vector<std::pair<std::string, double>>& ingredientVolumes
) {
    double totalVolume = 0;

    for(const auto& ingredientVolume : ingredientVolumes) {
        totalVolume += ingredientVolume.second;
    }

    std::vector<std::pair<std::string, double>> ingredientRatios;

    if(totalVolume > 0) {
        for(const auto& ingredientVolume : ingredientVolumes) {
            ingredientRatios.emplace_back(ingredientVolume.first, ingredientVolume.second / totalVolume);
        }
    }

    return {name, menuPrice, totalVolume, ingredientRatios};
}

MenuRegistry& MenuRegistry::getInstance() {
    static MenuRegistry instance;
    return instance;
}

const std::vector<Recipe>& MenuRegistry::getRecipes() const {
    return recipes;
}

const Recipe* MenuRegistry::findRecipe(const std::string& name) const {
    const auto foundRecipe = std::find_if(recipes.begin(), recipes.end(), [&name](const Recipe& recipe) {
        return recipe.getName() == name;
    });

    if(foundRecipe == recipes.end()) {
        return nullptr;
    }

    return &(*foundRecipe);
}

Recipe MenuRegistry::getRandomRecipe() const {
    if(recipes.empty()) {
        return {};
    }

    static std::random_device randomDevice;
    static std::mt19937 generator(randomDevice());

    std::uniform_int_distribution<std::size_t> recipeDistribution(0, recipes.size() - 1);
    return recipes[recipeDistribution(generator)];
}

#include "MenuRegistry.h"

#include <algorithm>
#include <cstddef>
#include <ostream>
#include <random>
#include <ranges>
#include <utility>

MenuRegistry::MenuRegistry() {
    addRecipe("Gin Tonic", 8, {
        {"Gin", 50, "ml"},
        {"Ice", 3, "cubes"},
        {"Tonic", 200, "ml"},
        {"Lemon", 1, "slice"},
    });

    addRecipe("Vermouth Spritz", 10, {
        {"Vermouth", 100, "ml"},
        {"Ice", 3, "cubes"},
        {"Soda", 200, "ml"},
        {"Grapefruit", 1, "slice"},
    });

    addRecipe("Screwdriver", 8, {
        {"Vodka", 50, "ml"},
        {"Orange Juice", 150, "ml"},
        {"Ice", 3, "cubes"},
    });

    addRecipe("Vodka Tonic", 8, {
        {"Vodka", 50, "ml"},
        {"Tonic", 150, "ml"},
        {"Lemon", 1, "slice"},
        {"Ice", 3, "cubes"},
    });

    addRecipe("Whiskey Sour", 10, {
        {"Whiskey", 60, "ml"},
        {"Lemon", 1, "slice"},
        {"Simple Syrup", 20, "ml"},
        {"Ice", 2, "cubes"},
    });

    addRecipe("Moscow Mule", 10, {
        {"Vodka", 50, "ml"},
        {"Ginger Beer", 150, "ml"},
        {"Lime", 1, "wedge"},
        {"Ice", 3, "cubes"},
    });

    addRecipe("Cuba Libre", 9, {
        {"Rum", 50, "ml"},
        {"Cola", 150, "ml"},
        {"Lime", 1, "wedge"},
        {"Ice", 3, "cubes"},
    });

    addRecipe("Tequila Sunrise", 10, {
        {"Tequila", 50, "ml"},
        {"Orange Juice", 150, "ml"},
        {"Grenadine", 20, "ml"},
        {"Ice", 3, "cubes"},
    });

    addRecipe("Mimosa", 9, {
        {"Sparkling Wine", 100, "ml"},
        {"Orange Juice", 100, "ml"},
        {"Ice", 2, "cubes"},
    });

    addRecipe("Aperol Spritz", 12, {
        {"Aperol", 75, "ml"},
        {"Sparkling Wine", 100, "ml"},
        {"Soda", 50, "ml"},
        {"Orange", 1, "slice"},
        {"Ice", 3, "cubes"},
    });
}

void MenuRegistry::addRecipe(
    const std::string& name,
    double menuPrice,
    const std::vector<MenuIngredientDisplay>& displayIngredients
) {
    std::vector<std::pair<std::string, double>> ingredientVolumes;

    for(const MenuIngredientDisplay& ingredient : displayIngredients) {
        ingredientVolumes.emplace_back(ingredient.name, convertDisplayAmountToMl(ingredient));
    }

    recipes.push_back(makeRecipeFromVolumes(name, menuPrice, ingredientVolumes));
    displayIngredientsByRecipe[name] = displayIngredients;
}

double MenuRegistry::convertDisplayAmountToMl(const MenuIngredientDisplay& ingredient) {
    if(ingredient.unit == "ml") {
        return ingredient.amount;
    }

    if(ingredient.name == "Ice") {
        return ingredient.amount * 25;
    }

    if(ingredient.name == "Lemon" || ingredient.name == "Lime") {
        return ingredient.amount * 10;
    }

    if(ingredient.name == "Grapefruit" || ingredient.name == "Orange") {
        return ingredient.amount * 15;
    }

    return ingredient.amount;
}

Recipe MenuRegistry::makeRecipeFromVolumes(
    const std::string& name,
    double menuPrice,
    const std::vector<std::pair<std::string, double>>& ingredientVolumes
) {
    double totalVolume = 0;

    for(const auto& val : ingredientVolumes | std::views::values) {
        totalVolume += val;
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
    const auto foundRecipe = std::ranges::find_if(recipes, [&name](const Recipe& recipe) {
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

void MenuRegistry::printMenu(std::ostream& out) const {
    out << "Menu:\n";

    for(const Recipe& recipe : recipes) {
        out << recipe.getName() << " - $" << recipe.getMenuPrice() << ": ";

        const auto foundDisplayIngredients = displayIngredientsByRecipe.find(recipe.getName());

        if(foundDisplayIngredients != displayIngredientsByRecipe.end()) {
            const std::vector<MenuIngredientDisplay>& ingredients = foundDisplayIngredients->second;

            for(std::size_t index = 0; index < ingredients.size(); ++index) {
                if(index != 0) {
                    out << ", ";
                }

                const MenuIngredientDisplay& ingredient = ingredients[index];
                out << ingredient.name << " " << ingredient.amount;

                if(ingredient.unit == "ml") {
                    out << "ml";
                } else {
                    out << " " << ingredient.unit;
                }
            }
        }

        out << "\n";
    }
}

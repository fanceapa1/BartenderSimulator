#ifndef BARTENDER_MENU_REGISTRY_H
#define BARTENDER_MENU_REGISTRY_H

#include "Drink.h"

#include <string>
#include <utility>
#include <vector>

class MenuRegistry {
private:
    std::vector<Recipe> recipes;

    MenuRegistry();
    static Recipe makeRecipeFromVolumes(const std::string& name,
                                        double menuPrice,
                                        const std::vector<std::pair<std::string, double>>& ingredientVolumes);

public:
    MenuRegistry(const MenuRegistry& other) = delete;
    MenuRegistry& operator=(const MenuRegistry& other) = delete;

    static MenuRegistry& getInstance();

    [[nodiscard]] const std::vector<Recipe>& getRecipes() const;
    [[nodiscard]] const Recipe* findRecipe(const std::string& name) const;
    [[nodiscard]] Recipe getRandomRecipe() const;
};

#endif

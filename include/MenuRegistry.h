#ifndef BARTENDER_MENU_REGISTRY_H
#define BARTENDER_MENU_REGISTRY_H

#include "Drink.h"

#include <iosfwd>
#include <map>
#include <string>
#include <utility>
#include <vector>

class MenuRegistry {
private:
    struct MenuIngredientDisplay {
        std::string name;
        double amount;
        std::string unit;
    };

    std::vector<Recipe> recipes;
    std::map<std::string, std::vector<MenuIngredientDisplay>> displayIngredientsByRecipe;

    MenuRegistry();
    void addRecipe(const std::string& name,
                   double menuPrice,
                   const std::vector<MenuIngredientDisplay>& displayIngredients);
    static Recipe makeRecipeFromVolumes(const std::string& name,
                                        double menuPrice,
                                        const std::vector<std::pair<std::string, double>>& ingredientVolumes);
    static double convertDisplayAmountToMl(const MenuIngredientDisplay& ingredient);

public:
    MenuRegistry(const MenuRegistry& other) = delete;
    MenuRegistry& operator=(const MenuRegistry& other) = delete;

    static MenuRegistry& getInstance();

    [[nodiscard]] const std::vector<Recipe>& getRecipes() const;
    [[nodiscard]] const Recipe* findRecipe(const std::string& name) const;
    [[nodiscard]] Recipe getRandomRecipe() const;

    void printMenu(std::ostream& out) const;
};

#endif

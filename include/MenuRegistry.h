#ifndef BARTENDER_MENU_REGISTRY_H
#define BARTENDER_MENU_REGISTRY_H

#include "Drink.h"

#include <string>
#include <vector>

class MenuRegistry {
private:
    std::vector<Recipe> recipes;

    MenuRegistry();

public:
    MenuRegistry(const MenuRegistry& other) = delete;
    MenuRegistry& operator=(const MenuRegistry& other) = delete;

    static MenuRegistry& getInstance();

    // TODO: add recipe loading, lookup, and menu access.
};

#endif

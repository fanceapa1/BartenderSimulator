#include <iostream>
#include <sstream>

#include "Drink.h"

int main() {
    std::istringstream recipeInput(R"(Gin Tonic
        Gin
        50
        Tonic
        190
        Ice
        4
        Lemon
        1)");

    Recipe recipe;
    recipeInput >> recipe;

    std::cout << recipe;
    std::cout << "Calculated ABV decimal: " << recipe.getABV() << "\n";

    return 0;
}

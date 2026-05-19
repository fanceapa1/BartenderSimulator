#include <iostream>
#include <sstream>
#include <string>

#include "Drink.h"

int main() {
    Concoction currentDrink;
    std::string command;

    while(std::cout << "Select action (pour/serve/discard):\n" && std::getline(std::cin, command)) {
        if(command == "pour") {
            std::string ingredientName;
            std::string amountText;

            std::cout << "Select ingredient to pour:\n";
            if(!std::getline(std::cin, ingredientName)) {
                std::cout << "Incomplete pour command.\n";
                break;
            }

            std::cout << "Enter amount:\n";
            if(!std::getline(std::cin, amountText)) {
                std::cout << "Incomplete pour command.\n";
                break;
            }

            std::istringstream amountInput(amountText);
            int amount = 0;

            if(!(amountInput >> amount)) {
                std::cout << "Invalid amount. Nothing was poured.\n";
                continue;
            }

            currentDrink.pour(ingredientName, amount);
            std::cout << "Current volume: " << currentDrink.getTotalVolume() << " ml\n";
            std::cout << "Current ABV: " << currentDrink.getABV() * 100 << "%\n";
            currentDrink.printIngredients(std::cout);
        } else if(command == "serve") {
            std::cout << "Drink served.\n";
            currentDrink.reset();
        } else if(command == "discard" || command == "dispose") {
            std::cout << "Drink disposed.\n";
            currentDrink.reset();
        } else if(command == "exit") {
            break;
        } else if(command.empty()) {
            continue;
        } else {
            std::cout << "Invalid command: " << command << "\n";
        }
    }

    return 0;
}

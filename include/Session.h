#ifndef BARTENDER_SESSION_H
#define BARTENDER_SESSION_H

#include "Customer.h"
#include "Drink.h"

#include <map>
#include <memory>
#include <string>
#include <vector>

class Session {
private:
    int currentMinuteOfShift;
    double dailyProfit;
    std::map<std::string, std::unique_ptr<Customer>> customers;
    std::vector<Recipe> servedRecipes;
    int completedCustomerCount;
    int successfulOrderCount;
    Customer* currentCustomer;
    Concoction currentDrink;
    Recipe lastServedRecipe;
    Concoction lastServedConcoction;
    bool hasLastServedOrder;
    bool leaderboardFinalized;
    std::string lastMessage;

    static double totalBarEarnings;

    void createCustomerPool();
    void pickNextCustomer();
    void printCurrentCustomer(bool repeatsLastOrder) const;
    void handlePour();
    void handleServe();
    void handleServeLast();
    void handleDiscard();
    void handleRefuse();
    static void handleMenu();
    void advanceTime();
    void finalizeSession();

    [[nodiscard]] bool isShiftComplete() const;

public:
    Session();

    void runTerminal();

    // GUI Hook methods
    [[nodiscard]] const Customer* getCurrentCustomer() const { return currentCustomer; }
    [[nodiscard]] const Concoction& getCurrentDrink() const { return currentDrink; }
    [[nodiscard]] int getSuccessfulOrderCount() const { return successfulOrderCount; }
    [[nodiscard]] bool getIsShiftComplete() const { return isShiftComplete(); }
    [[nodiscard]] const std::string& getLastMessage() const { return lastMessage; }
    void setLastMessage(const std::string& msg) { lastMessage = msg; }
    void clearMessage() { lastMessage.clear(); }

    void pourIngredient(const std::string& ingredientName, int amount);
    void serveCurrentDrink();
    void serveLastDrink();
    void discardCurrentDrink();
    void refuseCurrentCustomer();

    [[nodiscard]] double getDailyProfit() const;
    [[nodiscard]] std::string getCurrentClockTime() const;

    static void displayLeaderboard();
};

#endif

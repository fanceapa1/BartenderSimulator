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
    Customer* currentCustomer;
    Concoction currentDrink;

    static double totalBarEarnings;

    void createCustomerPool();
    void pickNextCustomer();
    void printCurrentCustomer() const;
    void handlePour();
    void handleServe();
    void handleDiscard();
    void handleRefuse();
    static void handleMenu();
    void advanceTime();

    [[nodiscard]] bool isShiftComplete() const;

public:
    Session();

    void run();

    [[nodiscard]] double getDailyProfit() const;
    [[nodiscard]] std::string getCurrentClockTime() const;

    static void displayLeaderboard();
};

#endif

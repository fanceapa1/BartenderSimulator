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

    static double totalBarEarnings;

public:
    // TODO: add shift/time, payment, serve(), and refuseService() behavior.
    static void displayLeaderboard();
};

#endif

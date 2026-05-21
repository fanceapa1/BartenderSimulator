#include "Session.h"

#include "CustomerFactory.h"
#include "Exceptions.h"
#include "Leaderboard.h"
#include "MenuRegistry.h"

#include <algorithm>
#include <cstddef>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <random>
#include <sstream>
#include <string>
#include <vector>

namespace {
constexpr int ShiftLengthMinutes = 4 * 60;
constexpr int MinutesPerCustomer = 15;
constexpr int MaxCustomerSlots = ShiftLengthMinutes / MinutesPerCustomer;
constexpr double SuccessfulOrderSatisfactionThreshold = 7.5;
constexpr double RepeatLastOrderChance = 0.15;
constexpr const char* LeaderboardFilename = "leaderboard.txt";
constexpr const char* SuccessfulOrdersLeaderboardFilename = "leaderboardNOrders.txt";

std::mt19937& randomGenerator() {
    static std::random_device randomDevice;
    static std::mt19937 generator(randomDevice());
    return generator;
}

bool rollBadDrunkEvent(const Customer& customer) {
    std::bernoulli_distribution badEventDistribution(customer.getDrunkEventBadChance());
    return badEventDistribution(randomGenerator());
}

bool rollRepeatLastOrder() {
    std::bernoulli_distribution repeatOrderDistribution(RepeatLastOrderChance);
    return repeatOrderDistribution(randomGenerator());
}

std::vector<std::string> makePossibleNames() {
    return {
        "Alex", "Maya", "Victor", "Elena", "Andrei", "Sofia", "Matei", "Irina", "David", "Ana",
        "Radu", "Maria", "Luca", "Ioana", "Stefan", "Daria", "Mihai", "Clara", "George", "Bianca",
        "Vlad", "Diana", "Paul", "Nora", "Tudor", "Laura", "Eric", "Sara", "Cristian", "Oana",
        "Robert", "Eva", "Dan", "Amalia", "Sebastian", "Ilinca", "Bogdan", "Teodora", "Razvan", "Mara",
        "Filip", "Adriana", "Octavian", "Carmen", "Ionut", "Larisa", "Silviu", "Monica", "Calin", "Alina",
    };
}
}

double Session::totalBarEarnings = 0;

Session::Session()
    : currentMinuteOfShift(0),
      dailyProfit(0),
      completedCustomerCount(0),
      successfulOrderCount(0),
      currentCustomer(nullptr),
      currentDrink(),
      lastServedRecipe(),
      lastServedConcoction(),
      hasLastServedOrder(false),
      leaderboardFinalized(false) {
    createCustomerPool();
    pickNextCustomer();
}

void Session::run() {
    std::string command;

    while(!isShiftComplete() && !customers.empty() &&
          std::cout << "Select action (pour/serve/serve_last/discard/refuse/menu):\n" && std::getline(std::cin, command)) {
        try {
            if(command == "pour") {
                handlePour();
            } else if(command == "serve") {
                handleServe();
            } else if(command == "serve_last") {
                handleServeLast();
            } else if(command == "discard" || command == "dispose") {
                handleDiscard();
            } else if(command == "refuse") {
                handleRefuse();
            } else if(command == "menu") {
                handleMenu();
            } else if(command == "exit") {
                break;
            } else if(command.empty()) {
                continue;
            } else {
                throw InvalidCommandException();
            }
        } catch(const GameRuleException& exception) {
            std::cout << exception.what() << "\n";
        }
    }

    if(isShiftComplete()) {
        std::cout << "Shift complete. Final time: " << getCurrentClockTime() << "\n";
    }

    if(customers.empty() && !isShiftComplete()) {
        std::cout << "No customers left in the bar. Shift ended early.\n";
    }

    finalizeSession();
}

double Session::getDailyProfit() const {
    return dailyProfit;
}

std::string Session::getCurrentClockTime() const {
    const int hour = currentMinuteOfShift / 60;
    const int minute = currentMinuteOfShift % 60;

    std::ostringstream out;
    out << std::setw(2) << std::setfill('0') << hour << ":"
        << std::setw(2) << std::setfill('0') << minute;
    return out.str();
}

void Session::displayLeaderboard() {
    Leaderboard<double> earningsLeaderboard;
    earningsLeaderboard.loadFromFile(LeaderboardFilename);
    earningsLeaderboard.print(std::cout, "Shift earnings leaderboard", "$");

    std::cout << "\n";

    Leaderboard<int> successfulOrdersLeaderboard;
    successfulOrdersLeaderboard.loadFromFile(SuccessfulOrdersLeaderboardFilename);
    successfulOrdersLeaderboard.print(std::cout, "Successful orders leaderboard", " successful orders");
}

void Session::createCustomerPool() {
    std::vector<std::string> possibleNames = makePossibleNames();
    std::ranges::shuffle(possibleNames, randomGenerator());

    std::uniform_int_distribution<int> poolSizeDistribution(10, 20);
    std::uniform_int_distribution<int> customerTypeDistribution(0, 2);
    const int poolSize = poolSizeDistribution(randomGenerator());

    for(int index = 0; index < poolSize && index < static_cast<int>(possibleNames.size()); ++index) {
        const std::string& customerName = possibleNames[index];
        customers.emplace(customerName, CustomerFactory::createCustomer(customerName, customerTypeDistribution(randomGenerator())));
    }
}

void Session::pickNextCustomer() {
    if(customers.empty()) {
        currentCustomer = nullptr;
        return;
    }

    std::uniform_int_distribution<std::size_t> customerDistribution(0, customers.size() - 1);
    auto customerIt = customers.begin();
    std::advance(customerIt, static_cast<std::ptrdiff_t>(customerDistribution(randomGenerator())));

    currentCustomer = customerIt->second.get();

    const bool repeatsLastOrder = hasLastServedOrder && rollRepeatLastOrder();

    if(repeatsLastOrder) {
        currentCustomer->setDrinkRequest(lastServedRecipe);
    } else {
        currentCustomer->chooseRandomDrinkRequest();
    }

    printCurrentCustomer(repeatsLastOrder);
}

void Session::printCurrentCustomer(bool repeatsLastOrder) const {
    if(currentCustomer == nullptr) {
        return;
    }

    std::cout << "Current time: " << getCurrentClockTime() << "\n";
    std::cout << currentCustomer->getName() << " - " << currentCustomer->getType();

    if(currentCustomer->isDrunk()) {
        std::cout << " - DRUNK";
    }

    std::cout << "\n";

    if(repeatsLastOrder) {
        std::cout << "Hey! I want what they just had!\n";
    } else {
        std::cout << "Hello! I would like a " << currentCustomer->getDrinkRequest().getName() << "!\n";
    }
}

void Session::handlePour() {
    std::string ingredientName;
    std::string amountText;

    std::cout << "Select ingredient to pour:\n";
    if(!std::getline(std::cin, ingredientName)) {
        std::cout << "Incomplete pour command.\n";
        return;
    }

    if(!Concoction::isKnownIngredient(ingredientName)) {
        throw InvalidIngredientException();
    }

    std::cout << "Enter amount:\n";
    if(!std::getline(std::cin, amountText)) {
        std::cout << "Incomplete pour command.\n";
        return;
    }

    std::istringstream amountInput(amountText);
    int amount = 0;

    if(!(amountInput >> amount)) {
        std::cout << "Invalid amount. Nothing was poured.\n";
        return;
    }

    currentDrink.pour(ingredientName, amount);
    std::cout << "Current volume: " << currentDrink.getTotalVolume() << " ml\n";
    std::cout << "Current ABV: " << currentDrink.getABV() * 100 << "%\n";
    currentDrink.printIngredients(std::cout);
}

void Session::handleServe() {
    if(currentCustomer == nullptr) {
        std::cout << "No customer is currently waiting.\n";
        return;
    }

    const std::string customerName = currentCustomer->getName();
    const bool wasAlreadyDrunk = currentCustomer->isDrunk();
    const Recipe requestedDrink = currentCustomer->getDrinkRequest();
    const double basePrice = requestedDrink.getMenuPrice();
    currentCustomer->receiveDrink(currentDrink);
    const double satisfaction = currentCustomer->getSatisfaction();

    servedRecipes.push_back(requestedDrink);

    if(satisfaction > SuccessfulOrderSatisfactionThreshold) {
        ++successfulOrderCount;
    }

    lastServedRecipe = requestedDrink;
    lastServedConcoction = currentDrink;
    hasLastServedOrder = true;

    std::cout << "Drink served.\n";
    std::cout << "Satisfaction: " << satisfaction << "\n";

    if(wasAlreadyDrunk) {
        if(rollBadDrunkEvent(*currentCustomer)) {
            const double stolenAmount = dailyProfit * 0.10;

            dailyProfit -= stolenAmount;
            totalBarEarnings -= stolenAmount;

            std::cout << customerName << " stole money from the tip jar!\n";
            std::cout << "Money received: -" << stolenAmount << "$ (-" << stolenAmount << "$ tips)\n";
        } else {
            const double paidAmount = 3 * basePrice;
            const double tips = paidAmount - basePrice;

            dailyProfit += paidAmount;
            totalBarEarnings += paidAmount;

            std::cout << customerName << " left a very generous tip!\n";
            std::cout << "Money received: " << paidAmount << "$ (" << tips << "$ tips)\n";
        }

        customers.erase(customerName);
        currentCustomer = nullptr;
    } else {
        const double paidAmount = (1 + (satisfaction - 5) * 0.1) * basePrice;
        const double tips = paidAmount - basePrice;

        dailyProfit += paidAmount;
        totalBarEarnings += paidAmount;

        std::cout << "Money received: " << paidAmount << "$ (" << tips << "$ tips)\n";
    }

    std::cout << "Total shift earnings: " << dailyProfit << "$\n\n";

    currentDrink.reset();
    advanceTime();

    if(!isShiftComplete() && !customers.empty()) {
        pickNextCustomer();
    }
}

void Session::handleServeLast() {
    if(!hasLastServedOrder) {
        std::cout << "No previous drink to serve.\n";
        return;
    }

    currentDrink = lastServedConcoction;
    std::cout << "Serving a copy of the last drink.\n";
    handleServe();
}

void Session::handleDiscard() {
    std::cout << "Drink disposed.\n";
    currentDrink.reset();
}

void Session::handleRefuse() {
    if(currentCustomer == nullptr) {
        std::cout << "No customer is currently waiting.\n";
        return;
    }

    std::cout << "Service refused.\n\n";
    currentDrink.reset();
    advanceTime();

    if(!isShiftComplete()) {
        pickNextCustomer();
    }
}

void Session::handleMenu() {
    MenuRegistry::getInstance().printMenu(std::cout);
}

void Session::finalizeSession() {
    if(leaderboardFinalized) {
        return;
    }

    leaderboardFinalized = true;

    std::cout << "Final shift earnings: " << getDailyProfit() << "$\n";
    std::cout << "Successful orders: " << successfulOrderCount << "\n";
    std::cout << "Enter leaderboard name:\n";

    std::string playerName;

    if(!std::getline(std::cin, playerName) || playerName.empty()) {
        playerName = "Anonymous";
    }

    Leaderboard<double> earningsLeaderboard;
    earningsLeaderboard.loadFromFile(LeaderboardFilename);
    earningsLeaderboard.addEntry(playerName, getDailyProfit());
    earningsLeaderboard.sortByScoreDescending();
    earningsLeaderboard.saveToFile(LeaderboardFilename);

    Leaderboard<int> successfulOrdersLeaderboard;
    successfulOrdersLeaderboard.loadFromFile(SuccessfulOrdersLeaderboardFilename);
    successfulOrdersLeaderboard.addEntry(playerName, successfulOrderCount);
    successfulOrdersLeaderboard.sortByScoreDescending();
    successfulOrdersLeaderboard.saveToFile(SuccessfulOrdersLeaderboardFilename);

    displayLeaderboard();
}

void Session::advanceTime() {
    ++completedCustomerCount;
    currentMinuteOfShift = completedCustomerCount * MinutesPerCustomer;
}

bool Session::isShiftComplete() const {
    return completedCustomerCount >= MaxCustomerSlots;
}

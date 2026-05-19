#include "Customer.h"

#include <algorithm>
#include <cmath>
#include <ranges>
#include <utility>

namespace {
double closenessScore(const double expected, const double actual) {
    const double denominator = std::max(std::abs(expected), 0.01);
    const double difference = std::abs(expected - actual);
    return std::max(0.0, 1.0 - difference / denominator);
}

double calculateIngredientMatchScore(const Recipe& recipe, const Concoction& drink) {
    const auto& recipeIngredients = recipe.getIngredientRatios();

    if(recipeIngredients.empty()) {
        return 0;
    }

    int matchingIngredients = 0;

    for(const auto& key : recipeIngredients | std::views::keys) {
        if(drink.containsIngredient(key)) {
            ++matchingIngredients;
        }
    }

    return static_cast<double>(matchingIngredients) / static_cast<double>(recipeIngredients.size());
}
}

Customer::Customer(std::string name, double intoxicationLimitMl, Recipe drinkRequest)
    : name(std::move(name)),
      satisfaction(10),
      alcoholConsumedMl(0),
      intoxicationLimitMl(intoxicationLimitMl),
      drinkRequest(std::move(drinkRequest)),
      hasPreviousOrder(false) {
}

const std::string& Customer::getName() const {
    return name;
}

double Customer::getSatisfaction() const {
    return satisfaction;
}

const Recipe& Customer::getDrinkRequest() const {
    return drinkRequest;
}

void Customer::setDrinkRequest(const Recipe& newDrinkRequest) {
    drinkRequest = newDrinkRequest;
}

double Customer::receiveDrink(const Concoction& drink) {
    const double ingredientScore = calculateIngredientMatchScore(drinkRequest, drink);
    const double abvScore = closenessScore(drinkRequest.getABV(), drink.getABV());
    const double sweetnessScore = closenessScore(drinkRequest.getSweetness(), drink.getSweetness());

    const double weightedScore = 0.50 * ingredientScore + 0.25 * abvScore + 0.25 * sweetnessScore;
    const double currentOrderSatisfaction = std::clamp(10.0 * weightedScore, 0.0, 10.0);
    const double previousSatisfaction = hasPreviousOrder ? satisfaction : 10.0;

    satisfaction = (previousSatisfaction + currentOrderSatisfaction) / 2.0;
    alcoholConsumedMl += drink.getABV() * drink.getTotalVolume();
    hasPreviousOrder = true;

    return satisfaction;
}

CasualPatron::CasualPatron(std::string name, Recipe drinkRequest)
    : Customer(std::move(name), 80, std::move(drinkRequest)) {
}

double CasualPatron::calculateTip(double drinkAccuracy) const {
    static_cast<void>(drinkAccuracy);
    return 0;
}

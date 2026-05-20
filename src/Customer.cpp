#include "Customer.h"

#include "MenuRegistry.h"

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

double calculateCasualWeightedScore(const Recipe& recipe, const Concoction& drink) {
    const double ingredientScore = calculateIngredientMatchScore(recipe, drink);
    const double abvScore = closenessScore(recipe.getABV(), drink.getABV());
    const double sweetnessScore = closenessScore(recipe.getSweetness(), drink.getSweetness());

    return 0.50 * ingredientScore + 0.25 * abvScore + 0.25 * sweetnessScore;
}

double calculateCriticClosenessScore(const double expected, const double actual) {
    const double normalCloseness = closenessScore(expected, actual);
    return 1.5 * normalCloseness * normalCloseness;
}
}

Customer::Customer(std::string name, double intoxicationLimitMl)
    : name(std::move(name)),
      satisfaction(10),
      alcoholConsumedMl(0),
      intoxicationLimitMl(intoxicationLimitMl),
      drinkRequest(MenuRegistry::getInstance().getRandomRecipe()),
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

void Customer::chooseRandomDrinkRequest() {
    drinkRequest = MenuRegistry::getInstance().getRandomRecipe();
}

double Customer::receiveDrink(const Concoction& drink) {
    const double currentOrderSatisfaction = std::max(0.0, calculateCurrentOrderSatisfaction(drink));
    const double previousSatisfaction = hasPreviousOrder ? satisfaction : 10.0;

    satisfaction = (previousSatisfaction + currentOrderSatisfaction) / 2.0;
    alcoholConsumedMl += drink.getABV() * drink.getTotalVolume();
    hasPreviousOrder = true;

    return satisfaction;
}

CasualPatron::CasualPatron(std::string name)
    : Customer(std::move(name), 80) {
}

double CasualPatron::calculateCurrentOrderSatisfaction(const Concoction& drink) const {
    return 10.0 * calculateCasualWeightedScore(drinkRequest, drink);
}

double CasualPatron::calculateTip(double drinkAccuracy) const {
    static_cast<void>(drinkAccuracy);
    return 0;
}

std::string CasualPatron::getType() const {
    return "Casual Patron";
}

HeavyDrinker::HeavyDrinker(std::string name)
    : Customer(std::move(name), 140) {
}

double HeavyDrinker::calculateCurrentOrderSatisfaction(const Concoction& drink) const {
    const double ingredientScore = calculateIngredientMatchScore(drinkRequest, drink);
    const double abvScore = closenessScore(drinkRequest.getABV(), drink.getABV());
    const double sweetnessScore = closenessScore(drinkRequest.getSweetness(), drink.getSweetness());

    return 10.0 * (0.70 * abvScore + 0.30 * ingredientScore + 0.25 * sweetnessScore);
}

double HeavyDrinker::calculateTip(double drinkAccuracy) const {
    static_cast<void>(drinkAccuracy);
    return 0;
}

std::string HeavyDrinker::getType() const {
    return "Heavy Drinker";
}

Critic::Critic(std::string name)
    : Customer(std::move(name), 60) {
}

double Critic::calculateCurrentOrderSatisfaction(const Concoction& drink) const {
    const double ingredientScore = calculateIngredientMatchScore(drinkRequest, drink);
    const double abvScore = calculateCriticClosenessScore(drinkRequest.getABV(), drink.getABV());
    const double sweetnessScore = calculateCriticClosenessScore(drinkRequest.getSweetness(), drink.getSweetness());

    return 10.0 * (0.50 * ingredientScore + 0.25 * abvScore + 0.25 * sweetnessScore);
}

double Critic::calculateTip(double drinkAccuracy) const {
    static_cast<void>(drinkAccuracy);
    return 0;
}

std::string Critic::getType() const {
    return "Critic";
}

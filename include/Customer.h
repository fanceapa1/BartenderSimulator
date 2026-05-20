#ifndef BARTENDER_CUSTOMER_H
#define BARTENDER_CUSTOMER_H

#include "Drink.h"

#include <string>

class Customer {
protected:
    std::string name;
    double satisfaction;
    double alcoholConsumedMl;
    double intoxicationLimitMl;
    Recipe drinkRequest;
    bool hasPreviousOrder;

    [[nodiscard]] virtual double calculateCurrentOrderSatisfaction(const Concoction& drink) const = 0;

public:
    Customer(std::string name, double intoxicationLimitMl, Recipe drinkRequest);
    virtual ~Customer() = default;

    [[nodiscard]] const std::string& getName() const;
    [[nodiscard]] double getSatisfaction() const;
    [[nodiscard]] const Recipe& getDrinkRequest() const;

    void setDrinkRequest(const Recipe& newDrinkRequest);
    double receiveDrink(const Concoction& drink);

    [[nodiscard]] virtual double calculateTip(double drinkAccuracy) const = 0;
    [[nodiscard]] virtual std::string getType() const = 0;
};

class CasualPatron : public Customer {
public:
    CasualPatron(std::string name, Recipe drinkRequest);

    [[nodiscard]] double calculateCurrentOrderSatisfaction(const Concoction& drink) const override;
    [[nodiscard]] double calculateTip(double drinkAccuracy) const override;
    [[nodiscard]] std::string getType() const override;
};

class HeavyDrinker : public Customer {
public:
    HeavyDrinker(std::string name, Recipe drinkRequest);

    [[nodiscard]] double calculateCurrentOrderSatisfaction(const Concoction& drink) const override;
    [[nodiscard]] double calculateTip(double drinkAccuracy) const override;
    [[nodiscard]] std::string getType() const override;
};

class Critic : public Customer {
public:
    Critic(std::string name, Recipe drinkRequest);

    [[nodiscard]] double calculateCurrentOrderSatisfaction(const Concoction& drink) const override;
    [[nodiscard]] double calculateTip(double drinkAccuracy) const override;
    [[nodiscard]] std::string getType() const override;
};

#endif

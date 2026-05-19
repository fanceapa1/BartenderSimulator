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

public:
    Customer(std::string name, double intoxicationLimitMl, Recipe drinkRequest);
    virtual ~Customer() = default;

    [[nodiscard]] const std::string& getName() const;
    [[nodiscard]] double getSatisfaction() const;
    [[nodiscard]] const Recipe& getDrinkRequest() const;

    void setDrinkRequest(const Recipe& newDrinkRequest);
    double receiveDrink(const Concoction& drink);

    [[nodiscard]] virtual double calculateTip(double drinkAccuracy) const = 0;
};

class CasualPatron : public Customer {
public:
    CasualPatron(std::string name, Recipe drinkRequest);

    [[nodiscard]] double calculateTip(double drinkAccuracy) const override;
};

class HeavyDrinker : public Customer {
};

class Critic : public Customer {
};

#endif

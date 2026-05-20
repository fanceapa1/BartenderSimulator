#ifndef BARTENDER_CUSTOMER_H
#define BARTENDER_CUSTOMER_H

#include "Drink.h"

#include <string>

class Customer {
protected:
    std::string name;
    double satisfaction;
    double alcoholDrank;
    double drunkThresholdMl;
    double drunkEventStartingBadChance;
    Recipe drinkRequest;
    bool hasPreviousOrder;

    [[nodiscard]] virtual double calculateCurrentOrderSatisfaction(const Concoction& drink) const = 0;

public:
    Customer(std::string name, double drunkThresholdMl, double drunkEventStartingBadChance);
    virtual ~Customer() = default;

    [[nodiscard]] const std::string& getName() const;
    [[nodiscard]] double getSatisfaction() const;
    [[nodiscard]] double getAlcoholDrank() const;
    [[nodiscard]] double getDrunkThresholdMl() const;
    [[nodiscard]] bool isDrunk() const;
    [[nodiscard]] double getDrunkEventBadChance() const;
    [[nodiscard]] const Recipe& getDrinkRequest() const;

    void setDrinkRequest(const Recipe& newDrinkRequest);
    void chooseRandomDrinkRequest();
    double receiveDrink(const Concoction& drink);

    [[nodiscard]] virtual std::string getType() const = 0;
};

class CasualPatron : public Customer {
public:
    explicit CasualPatron(std::string name);

    [[nodiscard]] double calculateCurrentOrderSatisfaction(const Concoction& drink) const override;
    [[nodiscard]] std::string getType() const override;
};

class HeavyDrinker : public Customer {
public:
    explicit HeavyDrinker(std::string name);

    [[nodiscard]] double calculateCurrentOrderSatisfaction(const Concoction& drink) const override;
    [[nodiscard]] std::string getType() const override;
};

class Critic : public Customer {
public:
    explicit Critic(std::string name);

    [[nodiscard]] double calculateCurrentOrderSatisfaction(const Concoction& drink) const override;
    [[nodiscard]] std::string getType() const override;
};

#endif

#ifndef BARTENDER_CUSTOMER_H
#define BARTENDER_CUSTOMER_H

#include <string>

class Customer {
protected:
    std::string name;
    double satisfaction;
    double alcoholConsumedMl;
    double intoxicationLimitMl;

public:
    virtual ~Customer() = default;

    virtual double calculateTip(double drinkAccuracy) const = 0;
};

class CasualPatron : public Customer {
};

class HeavyDrinker : public Customer {
};

class Critic : public Customer {
};

#endif

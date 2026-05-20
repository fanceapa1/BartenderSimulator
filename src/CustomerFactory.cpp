#include "CustomerFactory.h"

#include <memory>

std::unique_ptr<Customer> CustomerFactory::createCustomer(const std::string& name, const int typeSeed) {
    const int customerType = ((typeSeed % 3) + 3) % 3;

    if(customerType == 1) {
        return std::make_unique<HeavyDrinker>(name);
    }

    if(customerType == 2) {
        return std::make_unique<Critic>(name);
    }

    return std::make_unique<CasualPatron>(name);
}

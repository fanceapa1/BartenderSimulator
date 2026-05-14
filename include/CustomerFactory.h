#ifndef BARTENDER_CUSTOMER_FACTORY_H
#define BARTENDER_CUSTOMER_FACTORY_H

#include "Customer.h"

#include <memory>
#include <string>

class CustomerFactory {
public:
    static std::unique_ptr<Customer> createCustomer(const std::string& name, int typeSeed = 0);
};

#endif

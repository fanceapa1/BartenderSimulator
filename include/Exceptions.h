#ifndef BARTENDER_EXCEPTIONS_H
#define BARTENDER_EXCEPTIONS_H

#include <exception>
#include <string>

class GameRuleException : public std::exception {
protected:
    std::string message;
};

class GlassOverflowException : public GameRuleException {
    // TODO: throw this when pouring exceeds glass capacity.
};

class IntoxicatedCustomerException : public GameRuleException {
    // TODO: throw this when a customer should be refused service.
};

#endif

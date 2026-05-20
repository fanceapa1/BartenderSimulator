#ifndef BARTENDER_EXCEPTIONS_H
#define BARTENDER_EXCEPTIONS_H

#include <exception>
#include <string>

class GameRuleException : public std::exception {
protected:
    std::string message;

public:
    explicit GameRuleException(std::string message);

    [[nodiscard]] const char* what() const noexcept override;
};

class InvalidCommandException : public GameRuleException {
public:
    InvalidCommandException();
};

class InvalidIngredientException : public GameRuleException {
public:
    InvalidIngredientException();
};

class GlassOverflowException : public GameRuleException {
public:
    GlassOverflowException();
};


#endif

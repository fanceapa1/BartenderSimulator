#include "Exceptions.h"

#include <utility>

GameRuleException::GameRuleException(std::string message)
    : message(std::move(message)) {
}

const char* GameRuleException::what() const noexcept {
    return message.c_str();
}

InvalidCommandException::InvalidCommandException()
    : GameRuleException("Invalid command!") {
}

InvalidIngredientException::InvalidIngredientException()
    : GameRuleException("Invalid ingredient name!") {
}

GlassOverflowException::GlassOverflowException()
    : GameRuleException("Glass capacity (500ml) overflow!") {
}

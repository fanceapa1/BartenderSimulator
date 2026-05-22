#include "GameController.h"
#include "Exceptions.h"

GameController::GameController() : selectedAmount(25) {
}

void GameController::pourIngredient(const std::string& name) {
    try {
        session.pourIngredient(name, selectedAmount);
    } catch(const GameRuleException& e) {
        session.setLastMessage(e.what());
    }
}

void GameController::serve() {
    session.serveCurrentDrink();
}

void GameController::serveLast() {
    session.serveLastDrink();
}

void GameController::discard() {
    session.discardCurrentDrink();
}

void GameController::refuse() {
    session.refuseCurrentCustomer();
}

void GameController::setSelectedAmount(int amount) {
    selectedAmount = amount;
}

int GameController::getSelectedAmount() const {
    return selectedAmount;
}

const Session& GameController::getSession() const {
    return session;
}

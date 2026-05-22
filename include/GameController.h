#ifndef BARTENDER_GAME_CONTROLLER_H
#define BARTENDER_GAME_CONTROLLER_H

#include "Session.h"
#include <string>

class GameController {
private:
    Session session;
    int selectedAmount;

public:
    GameController();

    void pourIngredient(const std::string& name);
    void serve();
    void serveLast();
    void discard();
    void refuse();

    void setSelectedAmount(int amount);
    [[nodiscard]] int getSelectedAmount() const;

    [[nodiscard]] const Session& getSession() const;
};

#endif // BARTENDER_GAME_CONTROLLER_H

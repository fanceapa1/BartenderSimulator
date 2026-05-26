#ifndef BARTENDER_GAME_APP_H
#define BARTENDER_GAME_APP_H

#include "GameController.h"
#include "GuiAssets.h"
#include "Button.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <cstdint>

class GameApp {
private:
    sf::RenderWindow window;
    GameController controller;
    GuiAssets assets;

    std::vector<Button> ingredientButtons;
    std::vector<Button> actionButtons;
    std::vector<Button> amountButtons;

    bool isMenuOpen;
    bool isLeaderboardScreen;
    std::string leaderboardInput;

    void initUI();
    void processEvents();
    void render();
    void renderGameplay();
    void renderMenu();
    void renderLeaderboard();
    void handleMouseClick(const sf::Vector2f& mousePos);
    void handleTextEntered(std::uint32_t unicode);

public:
    GameApp();
    void run();
};

#endif // BARTENDER_GAME_APP_H

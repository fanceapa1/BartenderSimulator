#include "GameApp.h"
#include "MenuRegistry.h"
#include "Leaderboard.h"
#include <iostream>
#include <sstream>

GameApp::GameApp() : window(sf::VideoMode({1280, 720}), "Bartender Simulator GUI"), isMenuOpen(false), isLeaderboardScreen(false) {
    window.setFramerateLimit(60);
    assets.loadFont("assets/fonts/arial.ttf"); // Assume this is the fallback path or we will download one if needed
    initUI();
}

void GameApp::initUI() {
    if (!assets.isFontLoaded()) {
        std::cerr << "Warning: UI initialized without loaded font.\n";
        return;
    }

    const sf::Font& font = assets.getMainFont();

    // Ingredients (3 columns)
    std::vector<std::string> ingredients = {
        "Gin", "Vodka", "Rum", "Whiskey", "Tequila", "Vermouth", "Aperol", "Sparkling Wine",
        "Tonic", "Soda", "Orange Juice", "Cola", "Ginger Beer", "Simple Syrup", "Grenadine",
        "Ice", "Lemon", "Lime", "Orange", "Grapefruit"
    };

    float startX = 800.f;
    float startY = 100.f;
    for (size_t i = 0; i < ingredients.size(); ++i) {
        float x = startX + (i % 3) * 150.f;
        float y = startY + (i / 3) * 50.f;
        ingredientButtons.emplace_back(ingredients[i], font, sf::Vector2f(x, y), sf::Vector2f(140.f, 40.f));
    }

    // Amounts
    std::vector<std::pair<std::string, int>> amounts = {
        {"25ml", 25}, {"50ml", 50}, {"100ml", 100}, {"1 pc", 1}, {"2 pc", 2}, {"3 pc", 3}
    };
    for (size_t i = 0; i < amounts.size(); ++i) {
        float x = startX + (i % 3) * 100.f;
        float y = startY + 400.f + (i / 3) * 50.f;
        amountButtons.emplace_back(amounts[i].first, font, sf::Vector2f(x, y), sf::Vector2f(90.f, 40.f));
    }

    // Actions
    std::vector<std::string> actions = {"Serve", "Serve Last", "Discard", "Refuse", "Menu", "Exit"};
    for (size_t i = 0; i < actions.size(); ++i) {
        float x = 50.f + i * 150.f;
        float y = 650.f;
        actionButtons.emplace_back(actions[i], font, sf::Vector2f(x, y), sf::Vector2f(140.f, 40.f));
    }
}

void GameApp::run() {
    while (window.isOpen()) {
        processEvents();
        
        if (controller.getSession().getIsShiftComplete() && !isLeaderboardScreen) {
            isLeaderboardScreen = true;
        }

        render();
    }
}

void GameApp::processEvents() {
    while (const std::optional<sf::Event> optEvent = window.pollEvent()) {
        const sf::Event& event = *optEvent;

        if (event.is<sf::Event::Closed>()) {
            window.close();
        } else if (const auto* mouseMoved = event.getIf<sf::Event::MouseMoved>()) {
            sf::Vector2f mousePos(static_cast<float>(mouseMoved->position.x), static_cast<float>(mouseMoved->position.y));
            if (!isMenuOpen && !isLeaderboardScreen) {
                for (auto& btn : ingredientButtons) btn.update(mousePos);
                for (auto& btn : amountButtons) btn.update(mousePos);
                for (auto& btn : actionButtons) btn.update(mousePos);
            } else if (isMenuOpen) {
                // simple menu exit check
                actionButtons[4].update(mousePos); // "Menu" button to toggle
            }
        } else if (const auto* mouseButton = event.getIf<sf::Event::MouseButtonReleased>()) {
            if (mouseButton->button == sf::Mouse::Button::Left) {
                sf::Vector2f mousePos(static_cast<float>(mouseButton->position.x), static_cast<float>(mouseButton->position.y));
                handleMouseClick(mousePos);
            }
        } else if (const auto* textEvent = event.getIf<sf::Event::TextEntered>()) {
            if (isLeaderboardScreen) {
                handleTextEntered(textEvent->unicode);
            }
        }
    }
}

void GameApp::handleMouseClick(const sf::Vector2f& mousePos) {
    if (isLeaderboardScreen) return;

    if (isMenuOpen) {
        if (actionButtons[4].isClicked(mousePos, true)) { // Menu toggle
            isMenuOpen = false;
        }
        return;
    }

    for (const auto& btn : amountButtons) {
        if (btn.isClicked(mousePos, true)) {
            std::string label = btn.getLabel();
            int amount = 0;
            if (label == "25ml") amount = 25;
            else if (label == "50ml") amount = 50;
            else if (label == "100ml") amount = 100;
            else if (label == "1 pc") amount = 1;
            else if (label == "2 pc") amount = 2;
            else if (label == "3 pc") amount = 3;
            controller.setSelectedAmount(amount);
            // Optionally update UI to show selected amount
            return;
        }
    }

    for (const auto& btn : ingredientButtons) {
        if (btn.isClicked(mousePos, true)) {
            controller.pourIngredient(btn.getLabel());
            return;
        }
    }

    for (const auto& btn : actionButtons) {
        if (btn.isClicked(mousePos, true)) {
            std::string action = btn.getLabel();
            if (action == "Serve") controller.serve();
            else if (action == "Serve Last") controller.serveLast();
            else if (action == "Discard") controller.discard();
            else if (action == "Refuse") controller.refuse();
            else if (action == "Menu") isMenuOpen = true;
            else if (action == "Exit") window.close();
            return;
        }
    }
}

void GameApp::handleTextEntered(std::uint32_t unicode) {
    if (unicode == '\b' && !leaderboardInput.empty()) {
        leaderboardInput.pop_back();
    } else if (unicode == '\r' || unicode == '\n') {
        // Save to leaderboard
        Leaderboard<double> earningsLeaderboard;
        earningsLeaderboard.loadFromFile("leaderboard.txt");
        earningsLeaderboard.addEntry(leaderboardInput.empty() ? "Anonymous" : leaderboardInput, controller.getSession().getDailyProfit());
        earningsLeaderboard.sortByScoreDescending();
        earningsLeaderboard.saveToFile("leaderboard.txt");

        Leaderboard<int> successfulOrdersLeaderboard;
        successfulOrdersLeaderboard.loadFromFile("leaderboardNOrders.txt");
        successfulOrdersLeaderboard.addEntry(leaderboardInput.empty() ? "Anonymous" : leaderboardInput, controller.getSession().getSuccessfulOrderCount());
        successfulOrdersLeaderboard.sortByScoreDescending();
        successfulOrdersLeaderboard.saveToFile("leaderboardNOrders.txt");

        window.close();
    } else if (unicode >= 32 && unicode < 128) {
        leaderboardInput += static_cast<char>(unicode);
    }
}

void GameApp::render() {
    window.clear(sf::Color(30, 30, 30));

    if (isLeaderboardScreen) {
        renderLeaderboard();
    } else if (isMenuOpen) {
        renderMenu();
        actionButtons[4].render(window); // Render Menu button over it to close
    } else {
        renderGameplay();
    }

    window.display();
}

void GameApp::renderGameplay() {
    if (!assets.isFontLoaded()) return;

    sf::Text text(assets.getMainFont());
    text.setCharacterSize(20);
    text.setFillColor(sf::Color::White);

    // Top Bar Info
    std::ostringstream topBar;
    topBar << "Time: " << controller.getSession().getCurrentClockTime()
           << " | Profit: $" << controller.getSession().getDailyProfit()
           << " | Success: " << controller.getSession().getSuccessfulOrderCount()
           << " | Selected Amount: " << controller.getSelectedAmount();
    text.setString(topBar.str());
    text.setPosition({20.f, 20.f});
    window.draw(text);

    // Customer Info
    const Customer* cust = controller.getSession().getCurrentCustomer();
    if (cust) {
        std::ostringstream custInfo;
        custInfo << "Customer: " << cust->getName() << " (" << cust->getType() << ")";
        if (cust->isDrunk()) custInfo << " - DRUNK";
        custInfo << "\nRequest: " << cust->getDrinkRequest().getName();
        
        text.setString(custInfo.str());
        text.setPosition({20.f, 100.f});
        window.draw(text);
        
        sf::RectangleShape custSprite({200.f, 300.f});
        custSprite.setPosition({50.f, 200.f});
        custSprite.setFillColor(sf::Color(100, 150, 200));
        window.draw(custSprite);
    }

    // Glass Info
    const Concoction& drink = controller.getSession().getCurrentDrink();
    std::ostringstream glassInfo;
    glassInfo << "Glass:\nVol: " << drink.getTotalVolume() << " ml\n"
              << "ABV: " << (drink.getABV() * 100.f) << "%\n";
    text.setString(glassInfo.str());
    text.setPosition({350.f, 100.f});
    window.draw(text);

    sf::RectangleShape glassSprite({150.f, 250.f});
    glassSprite.setPosition({350.f, 250.f});
    glassSprite.setFillColor(sf::Color(255, 255, 255, 50));
    window.draw(glassSprite);

    // Fill level
    if (drink.getTotalVolume() > 0) {
        float fillHeight = std::min(250.f, static_cast<float>(drink.getTotalVolume()) / 2.f); // pseudo scale
        sf::RectangleShape fillSprite({150.f, fillHeight});
        fillSprite.setPosition({350.f, 500.f - fillHeight});
        fillSprite.setFillColor(sf::Color(50, 100, 255, 150));
        window.draw(fillSprite);
    }

    // Message Board
    text.setString(controller.getSession().getLastMessage());
    text.setPosition({20.f, 550.f});
    window.draw(text);

    for (const auto& btn : ingredientButtons) btn.render(window);
    for (const auto& btn : amountButtons) btn.render(window);
    for (const auto& btn : actionButtons) btn.render(window);
}

void GameApp::renderMenu() {
    if (!assets.isFontLoaded()) return;

    sf::Text text(assets.getMainFont());
    text.setCharacterSize(20);
    text.setFillColor(sf::Color::White);
    text.setPosition({50.f, 50.f});
    
    std::ostringstream menuText;
    menuText << "--- Menu ---\n\n";
    const auto& recipes = MenuRegistry::getInstance().getRecipes();
    for (const auto& recipe : recipes) {
        menuText << recipe.getName() << " - $" << recipe.getMenuPrice() << "\n";
    }
    
    text.setString(menuText.str());
    window.draw(text);
}

void GameApp::renderLeaderboard() {
    if (!assets.isFontLoaded()) return;

    sf::Text text(assets.getMainFont());
    text.setCharacterSize(30);
    text.setFillColor(sf::Color::White);
    text.setPosition({400.f, 200.f});
    
    std::ostringstream endText;
    endText << "Shift Complete!\nFinal Earnings: $" << controller.getSession().getDailyProfit() << "\n";
    endText << "Successful Orders: " << controller.getSession().getSuccessfulOrderCount() << "\n\n";
    endText << "Enter Name for Leaderboard:\n" << leaderboardInput << "_";
    
    text.setString(endText.str());
    window.draw(text);
}

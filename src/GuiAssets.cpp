#include "GuiAssets.h"
#include <iostream>

GuiAssets::GuiAssets() : fontLoaded(false) {}

bool GuiAssets::loadFont(const std::string& fontPath) {
    if(mainFont.openFromFile(fontPath)) { // Note: SFML 3 uses openFromFile for sf::Font
        fontLoaded = true;
        return true;
    }
    // Fallback: try loading some default system fonts if possible, or just fail.
    std::cerr << "Failed to load font from " << fontPath << "\n";
    return false;
}

void GuiAssets::loadTextures() {
    if (!backgroundTexture.loadFromFile("assets/background.png")) {
        std::cerr << "Failed to load background texture\n";
    }

    (void)customerTextures["Casual Patron_Male"].loadFromFile("assets/customers/casualPatronMale.png");
    (void)customerTextures["Casual Patron_Female"].loadFromFile("assets/customers/casualPatronFemale.png");
    (void)customerTextures["Heavy Drinker_Male"].loadFromFile("assets/customers/heavyDrinkerMale.png");
    (void)customerTextures["Heavy Drinker_Female"].loadFromFile("assets/customers/heavyDrinkerFemale.png");
    (void)customerTextures["Critic_Male"].loadFromFile("assets/customers/criticMale.png");
    (void)customerTextures["Critic_Female"].loadFromFile("assets/customers/criticFemale.png");

    auto formatIngredientName = [](std::string name) {
        std::string filename;
        for (char c : name) {
            if (c == ' ') filename += '_';
            else filename += static_cast<char>(std::tolower(c));
        }
        return "assets/ingredients/" + filename + ".png";
    };

    std::vector<std::string> ingredients = {
        "Gin", "Vodka", "Rum", "Whiskey", "Tequila", "Vermouth", "Aperol", "Sparkling Wine",
        "Tonic", "Soda", "Orange Juice", "Cola", "Ginger Beer", "Simple Syrup", "Grenadine",
        "Ice", "Lemon", "Lime", "Orange", "Grapefruit"
    };

    for (const auto& ing : ingredients) {
        if (!ingredientTextures[ing].loadFromFile(formatIngredientName(ing))) {
            std::cerr << "Failed to load ingredient: " << ing << "\n";
        }
    }
}

const sf::Texture* GuiAssets::getBackgroundTexture() const {
    return &backgroundTexture;
}

const sf::Texture* GuiAssets::getCustomerTexture(const std::string& type, bool isFemale) const {
    std::string key = type + (isFemale ? "_Female" : "_Male");
    auto it = customerTextures.find(key);
    if (it != customerTextures.end()) return &it->second;
    return nullptr;
}

const sf::Texture* GuiAssets::getIngredientTexture(const std::string& name) const {
    auto it = ingredientTextures.find(name);
    if (it != ingredientTextures.end()) return &it->second;
    return nullptr;
}

const sf::Font& GuiAssets::getMainFont() const {
    return mainFont;
}

bool GuiAssets::isFontLoaded() const {
    return fontLoaded;
}

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

const sf::Font& GuiAssets::getMainFont() const {
    return mainFont;
}

bool GuiAssets::isFontLoaded() const {
    return fontLoaded;
}

#ifndef BARTENDER_GUI_ASSETS_H
#define BARTENDER_GUI_ASSETS_H

#include <SFML/Graphics.hpp>
#include <string>

class GuiAssets {
private:
    sf::Font mainFont;
    bool fontLoaded;

public:
    GuiAssets();

    bool loadFont(const std::string& fontPath);
    [[nodiscard]] const sf::Font& getMainFont() const;
    [[nodiscard]] bool isFontLoaded() const;
};

#endif // BARTENDER_GUI_ASSETS_H

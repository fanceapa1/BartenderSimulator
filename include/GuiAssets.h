#ifndef BARTENDER_GUI_ASSETS_H
#define BARTENDER_GUI_ASSETS_H

#include <SFML/Graphics.hpp>
#include <string>
#include <map>

class GuiAssets {
private:
    sf::Font mainFont;
    bool fontLoaded;

    sf::Texture backgroundTexture;
    std::map<std::string, sf::Texture> customerTextures;
    std::map<std::string, sf::Texture> ingredientTextures;

public:
    GuiAssets();

    bool loadFont(const std::string& fontPath);
    void loadTextures();
    [[nodiscard]] const sf::Font& getMainFont() const;
    [[nodiscard]] bool isFontLoaded() const;

    const sf::Texture* getBackgroundTexture() const;
    const sf::Texture* getCustomerTexture(const std::string& type, bool isFemale) const;
    const sf::Texture* getIngredientTexture(const std::string& name) const;
};

#endif // BARTENDER_GUI_ASSETS_H

#ifndef BARTENDER_BUTTON_H
#define BARTENDER_BUTTON_H

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <string>

class Button {
private:
    sf::RectangleShape shape;
    sf::Text text;
    std::string labelStr;
    bool isHovered;

public:
    Button(const std::string& label, const sf::Font& font, const sf::Vector2f& position, const sf::Vector2f& size);

    void setPosition(const sf::Vector2f& position);
    void setSize(const sf::Vector2f& size);
    void setLabel(const std::string& label);

    void update(const sf::Vector2f& mousePos);
    void render(sf::RenderWindow& window) const;

    [[nodiscard]] bool isClicked(const sf::Vector2f& mousePos, bool mousePressed) const;
    [[nodiscard]] const std::string& getLabel() const;
};

#endif // BARTENDER_BUTTON_H

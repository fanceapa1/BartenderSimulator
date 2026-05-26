#include "Button.h"

Button::Button(const std::string& label, const sf::Font& font, const sf::Vector2f& position, const sf::Vector2f& size)
    : text(font, label, 16), labelStr(label), isHovered(false) {
    shape.setPosition(position);
    shape.setSize(size);
    shape.setFillColor(sf::Color(100, 100, 100));
    shape.setOutlineColor(sf::Color::White);
    shape.setOutlineThickness(2.f);

    text.setFillColor(sf::Color::White);

    // Center text
    sf::FloatRect textBounds = text.getLocalBounds();
    text.setOrigin({textBounds.position.x + textBounds.size.x / 2.0f, textBounds.position.y + textBounds.size.y / 2.0f});
    text.setPosition({position.x + size.x / 2.0f, position.y + size.y / 2.0f});
}

void Button::setPosition(const sf::Vector2f& position) {
    shape.setPosition(position);
    sf::FloatRect textBounds = text.getLocalBounds();
    text.setPosition({position.x + shape.getSize().x / 2.0f, position.y + shape.getSize().y / 2.0f});
}

void Button::setSize(const sf::Vector2f& size) {
    shape.setSize(size);
    setPosition(shape.getPosition());
}

void Button::setLabel(const std::string& label) {
    labelStr = label;
    text.setString(label);
    setPosition(shape.getPosition());
}

void Button::update(const sf::Vector2f& mousePos) {
    isHovered = shape.getGlobalBounds().contains(mousePos);
    if(isHovered) {
        shape.setFillColor(sf::Color(150, 150, 150));
    } else {
        shape.setFillColor(sf::Color(100, 100, 100));
    }
}

void Button::render(sf::RenderWindow& window) const {
    window.draw(shape);
    window.draw(text);
}

bool Button::isClicked(const sf::Vector2f& mousePos, bool mousePressed) const {
    return isHovered && mousePressed;
}

const std::string& Button::getLabel() const {
    return labelStr;
}

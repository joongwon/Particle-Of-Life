#include "Button.h"


#pragma warning(disable 26812)

void joongwon::Button::process_event(sf::Window &window, const sf::Event &e)
{
    switch (e.type) {
    case sf::Event::MouseButtonPressed:
        clicked = sprite_clicked.getGlobalBounds().contains(e.mouseButton.x, e.mouseButton.y);
        break;
    case sf::Event::MouseButtonReleased:
        if (clicked) {
            if (sprite_clicked.getGlobalBounds().contains(e.mouseButton.x, e.mouseButton.y)) {
                event_handler();
            }
            clicked = false;
        }
        break;
    case sf::Event::MouseMoved:
    {
        bool inBounds = sprite_clicked.getGlobalBounds().contains(e.mouseMove.x, e.mouseMove.y);
        sf::Cursor cursor;
        cursor.loadFromSystem(inBounds ? sf::Cursor::Hand : sf::Cursor::Arrow);
        window.setMouseCursor(cursor);
        if (clicked) {
            clicked = inBounds;
        }
    }
        break;
    default:
        break;
    }
}

void joongwon::Button::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
    target.draw(clicked ? sprite_clicked : sprite_unclicked, states);
}

#include "Button.h"


#pragma warning(disable : 26812)

void joongwon::Button::process_event(sf::Window &window, const sf::Event &e)
{
    switch (e.type) {
    case sf::Event::MouseButtonPressed:
    {
        clicked = in_bounds;
        if (clicked) {
            text.setFillColor(color_clicked);
        }
        break;
    }
    case sf::Event::MouseButtonReleased:
    {
        if (clicked) {
            if (text.getGlobalBounds().contains(e.mouseButton.x, e.mouseButton.y)) {
                event_handler(this);
            }
            clicked = false;
            text.setFillColor(color_unclicked);
        }
        break;
    }
    case sf::Event::MouseMoved:
    {
        bool now_in_bounds = text.getGlobalBounds().contains(e.mouseMove.x, e.mouseMove.y);
        if (now_in_bounds != in_bounds) {
            text.setStyle(now_in_bounds ? sf::Text::Style::Underlined : sf::Text::Style::Regular);
            if (clicked) {
                clicked = now_in_bounds;
            }
            in_bounds = now_in_bounds;
        }
    }
        break;
    default:
        return;
    }

}

void joongwon::Button::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
    target.draw(text, states);
}

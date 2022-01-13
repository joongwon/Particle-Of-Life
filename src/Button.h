#pragma once

#include <SFML/Graphics.hpp>
#include <functional>

namespace joongwon {
    class Button : public sf::Drawable
    {
    public:
        void process_event(sf::Window &window, const sf::Event &e);
        std::function<void(Button *)> event_handler;
        sf::Text text;
        sf::Color color_clicked = sf::Color::Green;
        sf::Color color_unclicked = sf::Color::White;
        bool clicked = false;
        bool in_bounds = false;

        // Inherited via Drawable
        virtual void draw(sf::RenderTarget & target, sf::RenderStates states) const override;
    };

}

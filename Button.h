#pragma once

#include <SFML/Graphics.hpp>
#include <functional>

namespace joongwon {
    class Button : public sf::Drawable
    {
    public:
        void process_event(sf::Window &window, const sf::Event &e);
        std::function<void()> event_handler;
        sf::Sprite sprite_clicked;
        sf::Sprite sprite_unclicked;
        bool clicked = false;

        // Inherited via Drawable
        virtual void draw(sf::RenderTarget & target, sf::RenderStates states) const override;
    };

}

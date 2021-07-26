#include "ParticleOfLife.h"
#include "myutil.h"
#include "Button.h"

#include <SFML/Graphics.hpp>

#include <iostream>
#include <vector>
#include <numeric>
#include <string>


int main()
{
    // configure
    joongwon::ParticleOfLifeConfig config;
    try {
        config.load_from_file("config.txt");
    }
    catch (const std::logic_error &e) {
        joongwon::showErrorMessageBox(e.what());
        std::exit(EXIT_FAILURE);
    }
    joongwon::ParticleOfLife life;
    life.configure(config);

    // generate
    life.generateRandomParticleTypes(config.type_count);
    life.generateRandomParticles(config.particle_count);

    // create window
    sf::Vector2f window_size(life.get_window_size());
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    std::cout << window_size.x << "," << window_size.y << std::endl;
    sf::RenderWindow window(sf::VideoMode(window_size.x, window_size.y), "Particle Of Life", sf::Style::Default, settings);
    sf::Image icon;
    icon.loadFromFile("ParticleOfLife.png");
    auto icon_size = icon.getSize();
    window.setIcon(icon_size.x, icon_size.y, icon.getPixelsPtr());

    // create fps display
    sf::Font font;
    font.loadFromFile("C:\\WINDOWS\\FONTS\\AGENCYR.TTF");
    sf::Text fps_display("", font);
    fps_display.setFillColor(sf::Color::White);

    // load buttons texture
    sf::Texture texture_button;
    texture_button.loadFromFile("buttons.png");

    // create refresh button
    joongwon::Button refresh_button;
    refresh_button.event_handler = [&]() {
        life.generateRandomParticleTypes(config.type_count);
        life.generateRandomParticles(config.particle_count);
    };
    refresh_button.sprite_clicked.setTexture(texture_button);
    refresh_button.sprite_clicked.setTextureRect({ 32, 0, 32, 32 });
    refresh_button.sprite_clicked.setPosition(80, 10);
    refresh_button.sprite_unclicked.setTexture(texture_button);
    refresh_button.sprite_unclicked.setTextureRect({ 0, 0, 32, 32 });
    refresh_button.sprite_unclicked.setPosition(80, 10);

    // variables about fps
    float seconds = 0;
    sf::Clock frame_counter;
    float minimum_frame_length = 0.005f;

    // event loop
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            refresh_button.process_event(window, event);
        }

        // next frame
        seconds += frame_counter.restart().asSeconds();
        if (seconds > minimum_frame_length) {
            life.advance(0.01);
            int fps = std::round(1 / seconds);
            fps_display.setString(std::to_string(fps));
            seconds = 0.f;
        }

        // redraw
        window.clear();
        window.draw(life);
        window.draw(fps_display);
        window.draw(refresh_button);
        window.display();
    }

    return 0;
}
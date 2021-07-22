#include "ParticleOfLife.h"
#include "myutil.h"

#include <SFML/Graphics.hpp>

#include <iostream>
#include <vector>
#include <numeric>
#include <string>


int main()
{
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

    life.generateRandomParticleTypes(config.type_count);
    sf::Clock clock;
    life.generateRandomParticles(config.particle_count);

    std::vector<sf::Time> grp_time;
    grp_time.push_back(clock.getElapsedTime());

    auto size = life.get_window_size();

    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    std::cout << size.x << "," << size.y << std::endl;
    sf::RenderWindow window(sf::VideoMode(size.x, size.y), "Particle Of Life", sf::Style::Default, settings);
    sf::Image icon;
    icon.loadFromFile("ParticleOfLife.png");
    auto icon_size = icon.getSize();
    window.setIcon(icon_size.x, icon_size.y, icon.getPixelsPtr());

    float seconds = 0;
    sf::Clock frame_counter;

    std::vector<sf::Time> adv_time;

    float minimum_frame_length = 0.005f;

    sf::Font font;
    font.loadFromFile("C:\\WINDOWS\\FONTS\\AGENCYR.TTF");
    sf::Text text("", font);
    text.setFillColor(sf::Color::White);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::MouseButtonReleased) {
                life.generateRandomParticleTypes(config.type_count);

                clock.restart();
                life.generateRandomParticles(config.particle_count);
                grp_time.push_back(clock.getElapsedTime());
            }
        }
        seconds += frame_counter.restart().asSeconds();
        if (seconds > minimum_frame_length) {
            clock.restart();
            life.advance(0.01f);
            int fps = std::round(1 / seconds);
            text.setString(std::to_string(fps));
            adv_time.push_back(clock.getElapsedTime());
            seconds = 0.f;
        }

        window.clear();
        window.draw(life);
        window.draw(text);
        window.display();
    }

    
    {
        auto [min, max] = std::minmax_element(grp_time.begin(), grp_time.end());
        auto sum = std::accumulate(grp_time.begin(), grp_time.end(), sf::Time::Zero);
        std::cout << "grp " << min->asSeconds() << ", " << sum.asSeconds() / grp_time.size() << ", " << max->asSeconds() << std::endl;
    }
    {
        auto [min, max] = std::minmax_element(adv_time.begin(), adv_time.end());
        auto sum = std::accumulate(adv_time.begin(), adv_time.end(), sf::Time::Zero);
        std::cout << "adv " << min->asSeconds() << ", " << sum.asSeconds() / adv_time.size() << ", " << max->asSeconds() << std::endl;
    }

    return 0;
}
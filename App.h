#pragma once

#include "ParticleOfLife.h"
#include "Button.h"

#include <SFML/Graphics.hpp>

#include <atomic>
#include <vector>
#include <future>

class App
{
    void configureLife();
    void createWindow();
    void createInterface();
    void addButton(const char *text, std::function<void(joongwon::Button *)> &&onclick);
    void exportParticleTypes();
    void importParticleTypes();

    joongwon::ParticleOfLife life;
    int types_count;
    int particles_count;
    std::unique_ptr<sf::RenderWindow> window;
    sf::Font font;
    sf::Text fps_display;
    std::atomic_bool pause;
    std::vector<std::unique_ptr<joongwon::Button>> buttons;
    float seconds = 0;
    sf::Clock frame_counter;
    sf::RectangleShape button_background;
    double minimum_frame_length;
    double maximum_frame_length;

public:
    void init();
    void runEventLoop();
};


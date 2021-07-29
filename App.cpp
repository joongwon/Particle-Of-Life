#include "App.h"

#include "apicall.h"

#include <fstream>

using namespace std::literals;

void App::configureLife()
{
    joongwon::ParticleOfLifeConfig config;
    try {
        config.load_from_file("config.txt");
    }
    catch (const std::logic_error &e) {
        joongwon::showErrorMessageBox(e.what());
        std::exit(EXIT_FAILURE);
    }
    life.configure(config);
    types_count = config.type_count;
    particles_count = config.particle_count;
    minimum_frame_length = 1. / config.maximum_fps;
    maximum_frame_length = 1. / config.minimum_fps;
}

void App::createWindow()
{
    auto window_size = life.get_window_size();
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    window.reset(new sf::RenderWindow(
        sf::VideoMode(window_size.x, window_size.y),
        "Particle Of Life",
        sf::Style::Titlebar | sf::Style::Close));
    sf::Image icon;
    icon.loadFromFile("icon.png");
    auto icon_size = icon.getSize();
    window->setIcon(icon_size.x, icon_size.y, icon.getPixelsPtr());
}

constexpr int ButtonHeight = 20;
constexpr int ButtonLeftMargin = 10;
constexpr int ButtonTopMargin = 40;

void App::createInterface()
{
    fps_display.setFont(font);
    addButton("New Types", [this](joongwon::Button *) {
        life.generateRandomParticleTypes(types_count);
        life.generateRandomParticles(particles_count);
    });
    addButton("New Particles", [this](joongwon::Button *) {
        life.generateRandomParticles(particles_count);
    });
    addButton("Export Particle Types", [this](joongwon::Button *) {
        exportParticleTypes();
    });
    addButton("Import Particle Types", [this](joongwon::Button *) {
        importParticleTypes();
    });
    addButton("Pause", [this](joongwon::Button *button) {
        pause = !pause;
        frame_counter.restart();
        button->text.setString(pause ? "Resume" : "Pause");
    });
    button_background.setPosition({ ButtonLeftMargin, ButtonTopMargin });
    float width = 0.f;
    for (auto &button : buttons) {
        auto bounds = button->text.getGlobalBounds();
        if (width < bounds.width)
            width = bounds.width;
    }
    button_background.setSize({ width, static_cast<float>(ButtonHeight * buttons.size()) });
    button_background.setFillColor(sf::Color(0, 0, 0, 0x80));
}

void App::addButton(const char *text, std::function<void(joongwon::Button *)> &&onclick)
{
    auto button = std::make_unique<joongwon::Button>();
    button->text.setCharacterSize(ButtonHeight);
    button->text.setFont(font);
    button->text.setPosition(ButtonLeftMargin, ButtonHeight * buttons.size() + ButtonTopMargin);
    button->text.setString(text);
    button->event_handler = std::move(onclick);
    buttons.push_back(std::move(button));
}

const char *PolFilter =
"Particle Of Life Type File (*.pol)\0" "*.pol\0"
"All Files (*.*)\0" "*.*\0"
;

void App::exportParticleTypes()
{
    try {
        auto file_name = joongwon::getSaveFileName(PolFilter);
        if (file_name.empty())
            throw 0;
        if (std::find(file_name.begin(), file_name.end(), '.') == file_name.end())
            file_name += ".pol";
        std::ofstream stream;
        stream.open(file_name);
        life.printTypesTo(stream);
        if (!stream.good())
            throw 1;
    }
    catch (int) {
        joongwon::showErrorMessageBox("Error occurred while exporting the particle types");
    }
}

void App::importParticleTypes()
{
    try {
        auto file_name = joongwon::getOpenFileName(PolFilter);
        if (file_name.empty())
            throw 0;
        std::ifstream stream;
        stream.open(file_name);
        life.scanTypesFrom(stream);
        if (!stream.good())
            throw 1;
        life.generateRandomParticles(particles_count);
    }
    catch (int) {
        joongwon::showErrorMessageBox("Error occurred while importing the particle types");
    }

}

void App::init()
{
    configureLife();
    life.generateRandomParticleTypes(types_count);
    life.generateRandomParticles(particles_count);

    font.loadFromFile("font.ttf");

    createWindow();
    createInterface();

    pause = false;
    seconds = 0;
}

void App::runEventLoop()
{
    frame_counter.restart();
    while (window->isOpen()) {
        sf::Event event;
        while (window->pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window->close();
            for (auto &button : buttons) {
                button->process_event(*window, event);
            }
        }

        if (!pause) {
            seconds += frame_counter.restart().asSeconds();
            if (seconds >= minimum_frame_length) {
                if (seconds > maximum_frame_length)
                    life.advance(maximum_frame_length);
                else
                    life.advance(seconds);
                int fps = std::round(1 / seconds);
                fps_display.setString("FPS: "s + std::to_string(fps));
                seconds = 0.f;
            }
        }

        // redraw
        window->clear();
        window->draw(life);
        window->draw(fps_display);
        window->draw(button_background);
        for (auto &button : buttons) {
            window->draw(*button);
        }
        window->display();
    }
}

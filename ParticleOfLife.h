#pragma once

#include "ParticleOfLifeConfig.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <list>

namespace joongwon {

    using Vector2d = sf::Vector2<double>;

struct Force {
    double a, b;
    double force(double x) const noexcept;
};

struct Particle {
    int type;
    Vector2d position;
    Vector2d velocity;
    int region;
};

class ParticleOfLife : public sf::Drawable, public sf::Transformable
{
    std::vector<Force> force_table_;
    std::vector<sf::Color> color_table_;
    std::vector<Particle> particles_;
    std::vector<int> region_borders;
    int types_count_ = 0;

    // Inherited via Drawable
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

    Vector2d acceleration(const Particle &p1, const Particle &p2) const noexcept;
    void accelerate(Particle &p, double dt);

public:
    double repulsion_strength = 100.f;
    double stablized_zone = 5.f;
    double effective_radius = 10.f;
    int regions_x_count = 10, regions_y_count = 10;
    double region_size = 50.f;
    double friction = 0.01f;
    double maximum_interaction_distance = 40.f;
    double maximum_interaction_strength = 10.f;
    double visible_radius = 5.f;
    ParticleOfLife();
    void configure(const ParticleOfLifeConfig &config);

    void generateRandomParticleTypes(int n);
    void generateRandomParticles(int n);
    void advance(double dt);

    Vector2d get_window_size() const noexcept;
    Vector2d get_size() const noexcept;

    void printTypesTo(std::ostream &os) const;
    void scanTypesFrom(std::istream &is);
};

}

#include "ParticleOfLife.h"

#include <random>
#include <cmath>
#include <utility>
#include <numbers>
#include <future>

#pragma warning(disable: 26451)


static std::default_random_engine random_engine(std::random_device{}());

void joongwon::ParticleOfLife::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    states.transform *= getTransform();

    auto size = get_size();
    sf::RectangleShape rect({ static_cast<float>(size.x), static_cast<float>(size.y) });
    rect.setFillColor(sf::Color::Black);
    target.draw(rect, states);

    sf::VertexArray va(sf::PrimitiveType::Lines);
    sf::Color grey(0x80, 0x80, 0x80);
    for (int i = 1; i < regions_x_count; i++) {
        va.append(sf::Vertex({ static_cast<float>(i * region_size), 0 }, grey));
        va.append(sf::Vertex({ static_cast<float>(i * region_size), static_cast<float>(size.y) }, grey));
    }
    for (int i = 1; i < regions_y_count; i++) {
        va.append(sf::Vertex({ 0, static_cast<float>(i * region_size) }, grey));
        va.append(sf::Vertex({ static_cast<float>(size.x), static_cast<float>(i * region_size) }, grey));
    }
    target.draw(va, states);

    for (auto &region : regions_) {
        for (auto &particle : region) {
            sf::CircleShape circle(2.5f);
            circle.move(sf::Vector2f(particle.position));
            circle.setFillColor(color_table_[particle.type]);
            target.draw(circle, states);
        }
    }
}

joongwon::Vector2d joongwon::ParticleOfLife::acceleration(const Particle &p1, const Particle &p2) const noexcept
{
    auto d = p1.position - p2.position;

    double x = std::sqrt(d.x * d.x + d.y * d.y);
    double magnitude;
    if (x > effective_radius + stablized_zone)
        magnitude = force_table_[p1.type * types_count_ + p2.type].force(x - effective_radius - stablized_zone);
    else if (x > effective_radius)
        magnitude = 0;
    else if (x > 0)
        magnitude = repulsion_strength * (effective_radius - x);
    else
        return { 0.f, 0.f };
    return d * (magnitude / x);
}

joongwon::ParticleOfLife::ParticleOfLife()
{
}

void joongwon::ParticleOfLife::configure(const ParticleOfLifeConfig &config)
{
    repulsion_strength = config.repulsion_strength;
    stablized_zone = config.stablized_zone;
    effective_radius = config.effective_radius;
    regions_x_count = config.regions_x_count;
    regions_y_count = config.regions_y_count;
    region_size = config.region_size;
    friction = config.friction;
    maximum_interaction_distance = config.maximum_interaction_distance;
    maximum_interaction_strength = config.maximum_interaction_strength;
}

void joongwon::ParticleOfLife::generateRandomParticleTypes(int n)
{
    if (region_size - effective_radius - stablized_zone < maximum_interaction_distance) {
        throw std::exception("invalid maximum effective distance");
    }
    types_count_ = n;
    force_table_.clear();
    color_table_.clear();

    std::uniform_real_distribution<> dist_scale(-1,1);
    std::uniform_real_distribution<float> dist_distance(0, maximum_interaction_distance);
    std::uniform_int_distribution<std::uint32_t> dist_color;
    
    for (int i = 0; i < n*n; i++) {
        force_table_.emplace_back(
            dist_scale(random_engine) * maximum_interaction_strength,
            dist_distance(random_engine)
        );
    }
    for (int i = 0; i < n; i++) {
        auto &a = force_table_[i * (n + 1)].a;
        if (a > 0) {
            // a = -a;
        }
    }

    for (int i = 0; i < n; i++) {
        auto c = dist_color(random_engine);
        c |= 0x80808080;
        sf::Color color{ c };
        color.a = 255;
        color_table_.push_back(std::move(color));
    }

    regions_.clear();
}

void joongwon::ParticleOfLife::generateRandomParticles(int n)
{
    regions_.assign(regions_x_count * regions_y_count, {});
    std::uniform_real_distribution<double> dist_x(0, regions_x_count * region_size);
    std::uniform_real_distribution<double> dist_y(0, regions_y_count * region_size);
    std::uniform_int_distribution<> dist_type(0, types_count_ - 1);
    for (int i = 0; i < n; i++) {
        Particle particle{
            .type = dist_type(random_engine),
            .position = {dist_x(random_engine), dist_y(random_engine)},
            .velocity = {0,0}
        };
        int ix = std::floor(particle.position.x / region_size);
        int iy = std::floor(particle.position.y / region_size);
        regions_[iy * regions_x_count + ix].push_back(std::move(particle));
    }
}

void joongwon::ParticleOfLife::advance(double dt)
{
    auto size = get_size();

#define POL_CONCURRENT

#ifdef POL_CONCURRENT
    std::vector<std::future<void>> futures;
#endif

    int n = std::thread::hardware_concurrency() * 1;
    for (int i = 0; i < n; i++) {
        int start = (regions_x_count * i / n);
        int end = (regions_x_count * (i + 1) / n);
#ifdef POL_CONCURRENT
        futures.push_back(std::async([=]() {
#endif
            for (int xme = start; xme < end; xme++) {
                    for (int yme = 0; yme < regions_y_count; yme++)
                        for (auto &me : regions_[yme * regions_x_count + xme])
                            for (int xd = -1; xd <= 1; xd++)
                                for (int yd = -1; yd <= 1; yd++) {
                                    Vector2d offset = { 0.f,0.f };
                                    int xyou = xme + xd;
                                    if (xyou < 0) {
                                        xyou += regions_x_count;
                                        offset.x += size.x;
                                    }
                                    else if (xyou >= regions_x_count) {
                                        xyou -= regions_x_count;
                                        offset.x -= size.x;
                                    }
                                    int yyou = yme + yd;
                                    if (yyou < 0) {
                                        yyou += regions_y_count;
                                        offset.y += size.y;
                                    }
                                    else if (yyou >= regions_y_count) {
                                        yyou -= regions_y_count;
                                        offset.y -= size.y;
                                    }
                                    for (auto you : regions_[yyou * regions_x_count + xyou]) {
                                        if (&me == &you) continue;
                                        you.position -= offset;
                                        me.velocity += acceleration(me, you) * dt;
                                    }
                                }
            }
#ifdef POL_CONCURRENT
        }));
#endif
    }
#ifdef POL_CONCURRENT
    for (auto &future : futures) {
        future.wait();
    }
#endif

    std::vector<std::pair<int, Particle>> moved_particles;
    for (int i = 0; i < regions_x_count * regions_y_count; i++) {
            auto &region = regions_[i];
            for (auto it = region.begin(); it != region.end(); ) {
                it->velocity *= (1 - friction);
                it->position += it->velocity * dt;
                it->position.x = std::fmod(it->position.x, size.x);
                it->position.y = std::fmod(it->position.y, size.y);
                if (it->position.x < 0)
                    it->position.x += size.x;
                if (it->position.y < 0)
                    it->position.y += size.y;

                int xnew = std::floor(it->position.x / region_size);
                int ynew = std::floor(it->position.y / region_size);
                int inew = ynew * regions_x_count + xnew;
                if (i != inew) {
                    moved_particles.emplace_back(std::move(inew), std::move(*it));
                    it = region.erase(it);
                }
                else {
                    it++;
                }
            }
        }
    for (auto &[i, particle] : moved_particles) {
        regions_[i].push_back(std::move(particle));
    }
}

joongwon::Vector2d joongwon::ParticleOfLife::get_window_size() const noexcept
{
    auto scale = getScale();
    return { scale.x * region_size * regions_x_count, scale.y * region_size * regions_y_count };
}

joongwon::Vector2d joongwon::ParticleOfLife::get_size() const noexcept
{
    return { region_size * regions_x_count, region_size * regions_y_count };
}

double joongwon::Force::force(double x) const noexcept
{
    return std::max(0., std::min(x, b - x)) * a;
}

#include "ParticleOfLife.h"

#include <random>
#include <cmath>
#include <utility>
#include <future>
#include <numeric>


#undef NDEBUG
#include <cassert>

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

    for (auto &particle : particles_) {
        if (particle.type == types_count_)
            continue;
        sf::CircleShape circle(visible_radius);
        circle.move(sf::Vector2f(particle.position));
        circle.setFillColor(color_table_[particle.type]);
        target.draw(circle, states);
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

void joongwon::ParticleOfLife::accelerate(Particle &particle, double dt)
{
    auto size = get_size();
    int x = std::floor(particle.position.x / region_size);
    int y = std::floor(particle.position.y / region_size);
    int i = y * regions_x_count + x;
    for (int xd = -1; xd <= 1; xd++)
        for (int yd = -1; yd <= 1; yd++) {
            Vector2d offset = { 0.f,0.f };
            int xyou = x + xd;
            if (xyou < 0) {
                xyou += regions_x_count;
                offset.x += size.x;
            }
            else if (xyou >= regions_x_count) {
                xyou -= regions_x_count;
                offset.x -= size.x;
            }
            int yyou = y + yd;
            if (yyou < 0) {
                yyou += regions_y_count;
                offset.y += size.y;
            }
            else if (yyou >= regions_y_count) {
                yyou -= regions_y_count;
                offset.y -= size.y;
            }

            int iyou = yyou * regions_x_count + xyou;
            for (int i = region_borders[iyou]; i < region_borders[iyou + 1]; i++) {
                auto you = particles_[i];
                if (&particle == &(particles_[i])) continue;
                you.position -= offset;
                particle.velocity += acceleration(particle, you) * dt;
            }
        }
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
    visible_radius = config.visible_radius;
}

void joongwon::ParticleOfLife::generateRandomParticleTypes(int n)
{
    if (region_size - effective_radius - stablized_zone < maximum_interaction_distance) {
        throw std::exception("invalid maximum effective distance");
    }
    types_count_ = n;
    force_table_.clear();
    color_table_.clear();

    std::uniform_real_distribution<> dist_scale(-1, 1);
    std::uniform_real_distribution<float> dist_distance(0, maximum_interaction_distance);
    std::uniform_int_distribution<std::uint32_t> dist_color;

    for (int i = 0; i < n * n; i++) {
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

    region_borders.clear();
}

void joongwon::ParticleOfLife::generateRandomParticles(int n)
{
    particles_.clear();

    std::uniform_real_distribution<double> dist_x(0, regions_x_count * region_size);
    std::uniform_real_distribution<double> dist_y(0, regions_y_count * region_size);
    std::uniform_int_distribution<> dist_type(0, types_count_ - 1);
    for (int i = 0; i < n; i++) {
        Particle particle{
            .type = dist_type(random_engine),
            .position = {dist_x(random_engine), dist_y(random_engine)},
            .velocity = {0,0}
        };
        int xnew = std::floor(particle.position.x / region_size);
        int ynew = std::floor(particle.position.y / region_size);
        int inew = ynew * regions_x_count + xnew;
        particle.region = inew;
        particles_.push_back(particle);
    }
}

void joongwon::ParticleOfLife::advance(double dt)
{
    auto size = get_size();

    std::sort(particles_.begin(), particles_.end(), [](auto a, auto b) { return a.region < b.region; });

    region_borders.assign(regions_x_count * regions_y_count + 1, 0);
    int last_region = 0;
    for (int i = 1; i < particles_.size(); i++) {
        while (last_region < particles_[i].region) {
            region_borders[++last_region] = i;
        }
    }
    region_borders.back() = particles_.size();

#define CONCURRENT

    std::vector<std::future<void>> futures;

    int n = std::thread::hardware_concurrency() * 3;
    std::vector<int> tasks_count = { 0 };
    for (int i = 1; i < region_borders.size(); i++) {
        int region_size = region_borders[i] - region_borders[i - 1];
        tasks_count.push_back(tasks_count.back() + region_size * region_size);
    }

    int region_begin, region_end = 0;
    int particle_begin, particle_end = 0;
    for (int i = 0; i < n; i++) {
        region_begin = region_end;
        int goal = tasks_count.back() * (i + 1) / n;
        for (; tasks_count[region_end] < goal; region_end++)
            continue;

        particle_begin = particle_end;
        particle_end = std::floor(region_borders[region_end - 1]
            + (region_borders[region_end] - region_borders[region_end - 1])
            * std::sqrt(((0. + goal - tasks_count[region_end - 1]) / (tasks_count[region_end] - tasks_count[region_end - 1]))));

#ifdef CONCURRENT
        futures.push_back(std::async([=]() {
#endif
            for (int j = particle_begin; j < particle_end; j++) {
                accelerate(particles_[j], dt);
            }
#ifdef CONCURRENT
            }));
#endif
    }
    while (!futures.empty()) {
        futures.back().wait();
        futures.pop_back();
    }

    for (auto &particle : particles_) {
        particle.velocity *= (1 - friction);
        particle.position += particle.velocity * dt;
        if (particle.position.x < 0)
            particle.position.x += size.x;
        else if (particle.position.x >= size.x)
            particle.position.x -= size.x;
        if (particle.position.y < 0)
            particle.position.y += size.y;
        else if (particle.position.y >= size.y)
            particle.position.y -= size.y;

        int xnew = std::floor(particle.position.x / region_size);
        int ynew = std::floor(particle.position.y / region_size);
        int inew = ynew * regions_x_count + xnew;
        particle.region = inew;
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

void joongwon::ParticleOfLife::printTypesTo(std::ostream &os) const
{
    using namespace std;
    auto previous_flags = os.flags();
    os << hex << hexfloat;

    // numbers
    os << types_count_ << endl;

    // color table
    for (auto &color : color_table_) {
        os << color.toInteger() << endl;
    }

    // force table
    for (auto &force : force_table_) {
        os << force.a << ' ' << force.b << endl;
    }

    os.flags(previous_flags);
}

void joongwon::ParticleOfLife::scanTypesFrom(std::istream &is)
{
    using namespace std;
    auto previous_flags = is.flags();
    is >> hex >> hexfloat;

    // numbers
    is >> types_count_;
    color_table_.resize(types_count_);
    force_table_.resize(types_count_ * types_count_);

    // color table
    for (auto &color : color_table_) {
        sf::Uint32 i;
        is >> i;
        color = sf::Color(i);
    }

    // force table
    for (auto &force : force_table_) {
        is >> force.a >> force.b;
    }

    is.flags(previous_flags);
}

double joongwon::Force::force(double x) const noexcept
{
    return std::max(0., std::min(x, b - x)) * a;
}

#include "ParticleOfLifeConfig.h"
#include <fstream>
#include <unordered_map>
#include <algorithm>
#include <stdexcept>

std::string trim(const std::string &s) {
    auto b = std::find_if_not(s.begin(), s.end(), std::isblank);
    auto e = std::find_if_not(s.rbegin(), s.rend(), std::isblank);
    return std::string(b, e.base());
}

void joongwon::ParticleOfLifeConfig::load_from_file(const std::string &file_name)
{
    using namespace std::literals;
    std::ifstream file(file_name);
    while (!file.eof()) {
        std::string key, value;
        std::getline(file, key, '=');
        std::getline(file, value, '\n');
        key = trim(key);
        value = trim(value);
        if (key.empty() || value.empty())
            break;
#define START_ASSIGN() if (false) ;
#define ASSIGN(x) else if (key == #x) x = std::stod(value);
#define END_ASSIGN() else throw std::logic_error("invalid configuration: "s + key);

        START_ASSIGN()
            ASSIGN(effective_radius)
            ASSIGN(friction)
            ASSIGN(maximum_interaction_distance)
            ASSIGN(maximum_interaction_strength)
            ASSIGN(region_size)
            ASSIGN(regions_x_count)
            ASSIGN(regions_y_count)
            ASSIGN(repulsion_strength)
            ASSIGN(stablized_zone)
            ASSIGN(type_count)
            ASSIGN(particle_count)
        END_ASSIGN()
    }
}

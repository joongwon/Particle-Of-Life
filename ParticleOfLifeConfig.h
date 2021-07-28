#pragma once

#include <string>

namespace joongwon {

struct ParticleOfLifeConfig
{
    double repulsion_strength = 100.f;
    double stablized_zone = 5.f;
    double effective_radius = 10.f;
    int regions_x_count = 10, regions_y_count = 10;
    double region_size = 50.f;
    double friction = 0.01f;
    double maximum_interaction_distance = 40.f;
    double maximum_interaction_strength = 10.f;
    int type_count = 5;
    int particle_count = 1000;
    int maximum_fps = 120;
    double visible_radius = 5.f;
    void load_from_file(const std::string &file_name);
};

}


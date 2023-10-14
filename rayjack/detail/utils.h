#pragma once

#include <glm/geometric.hpp>
#include <glm/vec3.hpp>
#include <limits>
#include <random>

using vec3 = glm::vec3;
using color = glm::vec3;
using point3 = glm::vec3;

// Constants
const float infinity = std::numeric_limits<float>::infinity();
const float pi = 3.14159265f;

// Utility Functions
inline float degreesToRadians(float degrees) { return degrees * pi / 180.0f; }

inline float random_float() {
    static std::uniform_real_distribution<float> distribution(0.0, 1.0);
    static std::mt19937 generator;
    return distribution(generator);
}



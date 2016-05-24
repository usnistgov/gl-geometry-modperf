#ifndef SPHERE_HPP
#define SPHERE_HPP

#include "geometry.hpp"

#include <glm/vec3.hpp>

struct Sphere {
    Sphere(Geometry& geom, float radius, const glm::vec3& center);
};

#endif


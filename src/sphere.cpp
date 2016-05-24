#include "sphere.hpp"

#include <glm/gtc/constants.hpp>

#include <cmath>

Sphere::Sphere(Geometry& geom, float radius, const glm::vec3& center) {
    // polygonal grid resolution
    const unsigned int w = 16;
    const unsigned int h = 8;

    // build vertex, normal, and texture coordinate arrays
    const unsigned int numvert = (w+1) * (h+1);
    const unsigned int numtri = 2*w*h;
    const unsigned int id = geom.addDraw(numvert, 3*numtri);

    const unsigned int basevert = geom.baseVert(id);
    for (unsigned int j=0, idx=basevert; j<=h; ++j) {
    for (unsigned int i=0; i<=w; ++idx, ++i) {
        const float u = static_cast<float>(i)/static_cast<float>(w);
        const float v = static_cast<float>(j)/static_cast<float>(h);

        // 3D position from spherical coordinates
        const float theta = glm::two_pi<float>() * u;
        const float phi = glm::pi<float>() * v;
        const float cx = std::cos(theta);
        const float sx = std::sin(theta);
        const float cy = std::cos(phi);
        const float sy = std::sin(phi);

        // uv, normal, and position
        *geom.uv(idx) = glm::vec2(u, v);
        *geom.norm(idx) = glm::vec3(cx*sy, sx*sy, cy);
        *geom.pos(idx) = radius * *geom.norm(idx);
    }
    }

    // build index array linking sets of three vertices into triangles
    // two triangles per square in the grid. Each vertex index is
    // essentially its unfolded grid array position. Be careful that
    // each triangle ends up in counter-clockwise order
    Geometry::Index* tris = geom.tris(id);
    for (Geometry::Index y=0, idx=0; y<h; ++y) {
    for (Geometry::Index x=0; x<w; ++x) {
        tris[idx++] = (w+1)* y    + x;
        tris[idx++] = (w+1)* y    + x+1;
        tris[idx++] = (w+1)*(y+1) + x+1;

        tris[idx++] = (w+1)* y    + x;
        tris[idx++] = (w+1)*(y+1) + x+1;
        tris[idx++] = (w+1)*(y+1) + x;
    }
    }

    // initialize per-model data
    Geometry::ModelUniforms* unif = geom.modelUniforms(id);
    unif->modelMats = Xform::translate(center);
    unif->color = glm::vec4(1.f, 0.f, 0.f, 1.f);
}


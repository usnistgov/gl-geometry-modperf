#include "xform.hpp"

#include <cmath>

// construct pair from matrix and inverse
Xform Xform::create(const glm::mat4x4& mat, const glm::mat4x4& inv) {
    Xform mp = {mat, inv};
    return mp;
}

// identity
Xform Xform::identity() {
    return create(glm::mat4x4(     1.f,      0.f,      0.f,      0.f,
                                   0.f,      1.f,      0.f,      0.f,
                                   0.f,      0.f,      1.f,      0.f,
                                   0.f,      0.f,      0.f,      1.f),
                  glm::mat4x4(     1.f,      0.f,      0.f,      0.f,
                                   0.f,      1.f,      0.f,      0.f,
                                   0.f,      0.f,      1.f,      0.f,
                                   0.f,      0.f,      0.f,      1.f)
                  );
}

Xform Xform::xrotate(float angle) {
    float c = std::cos(angle), s = std::sin(angle);
    return create(glm::mat4x4(     1.f,      0.f,      0.f,      0.f,
                                   0.f,        c,        s,      0.f,
                                   0.f,       -s,        c,      0.f,
                                   0.f,      0.f,      0.f,      1.f),
                  glm::mat4x4(     1.f,      0.f,      0.f,      0.f,
                                   0.f,        c,       -s,      0.f,
                                   0.f,        s,        c,      0.f,
                                   0.f,      0.f,      0.f,      1.f)
                  );
}

Xform Xform::yrotate(float angle) {
    float c = std::cos(angle), s = std::sin(angle);
    return create(glm::mat4x4(       c,      0.f,       -s,      0.f,
                                   0.f,      1.f,      0.f,      0.f,
                                     s,      0.f,        c,      0.f,
                                   0.f,      0.f,      0.f,      1.f),
                  glm::mat4x4(       c,      0.f,        s,      0.f,
                                   0.f,      1.f,      0.f,      0.f,
                                    -s,      0.f,        c,      0.f,
                                   0.f,      0.f,      0.f,      1.f)
                  );
}

Xform Xform::zrotate(float angle) {
    float c = std::cos(angle), s = std::sin(angle);
    return create(glm::mat4x4(       c,        s,      0.f,      0.f,
                                    -s,        c,      0.f,      0.f,
                                   0.f,      0.f,      1.f,      0.f,
                                   0.f,      0.f,      0.f,      1.f),
                  glm::mat4x4(       c,       -s,      0.f,      0.f,
                                    -s,        c,      0.f,      0.f,
                                   0.f,      0.f,      1.f,      0.f,
                                   0.f,      0.f,      0.f,      1.f)
                  );
}

Xform Xform::translate(const glm::vec3& t) {
    return create(glm::mat4x4(     1.f,      0.f,      0.f,      0.f,
                                   0.f,      1.f,      0.f,      0.f,
                                   0.f,      0.f,      1.f,      0.f,
                                   t.x,      t.y,      t.z,      1.f),
                  glm::mat4x4(     1.f,      0.f,      0.f,      0.f,
                                   0.f,      1.f,      0.f,      0.f,
                                   0.f,      0.f,      1.f,      0.f,
                                  -t.x,     -t.y,     -t.z,      1.f)
                  );
}

Xform Xform::scale(const glm::vec3& s) {
    return create(glm::mat4x4( s.x,      0.f,      0.f,      0.f,
                               0.f,      s.y,      0.f,      0.f,
                               0.f,      0.f,      s.z,      0.f,
                               0.f,      0.f,      0.f,      1.f),
                  glm::mat4x4( 1.f/s.x,  0.f,      0.f,      0.f,
                               0.f,      1.f/s.y,  0.f,      0.f,
                               0.f,      0.f,      1.f/s.z,  0.f,
                               0.f,      0.f,      0.f,      1.f)
                  );
}

// build 4x4 perspective matrix from field of view (in radians),
// x/y aspect ratio, and near/far clipping plans
Xform Xform::perspective(float fov, float aspect, float near, float far) {
    float t = std::tan(0.5f*fov), ta = t*aspect;
    float npf = near+far, nmf = near-far, nf2 = near*far*2.f;
    float nmfi = 1.f/nmf, nf2i = 1.f/nf2;
    return create(glm::mat4x4(  1.f/ta,      0.f,      0.f,      0.f,
                                   0.f,    1.f/t,      0.f,      0.f,
                                   0.f,      0.f, npf*nmfi,     -1.f,
                                   0.f,      0.f, nf2*nmfi,      1.f),
                  glm::mat4x4(      ta,      0.f,      0.f,      0.f,
                                   0.f,        t,      0.f,      0.f,
                                   0.f,      0.f,      0.f, nmf*nf2i,
                                   0.f,      0.f,     -1.f, npf*nf2i)
                  );
}


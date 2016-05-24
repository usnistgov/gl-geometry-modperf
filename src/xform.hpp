#ifndef XFORM_HPP
#define XFORM_HPP

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

// transformation as a pair of 4x4 matrices (matrix and inverse)
struct Xform {
    glm::mat4x4 matrix, inverse;

    static Xform create(const glm::mat4x4& xform, const glm::mat4x4& invxform);
    static Xform identity();

    static Xform xrotate(float angle);
    static Xform yrotate(float angle);
    static Xform zrotate(float angle);

    static Xform translate(const glm::vec3& t);

    static Xform scale(const glm::vec3& s);

    // new perspective matrix from field of view (in radians),
    // x/y aspect ratio, and near/far clipping planes
    static Xform perspective(float fov, float aspect, float near, float far);
};

// multiply matrices, multiply inverse in reverse order
inline Xform operator*(const Xform& m1, const Xform& m2) {
    return Xform::create(m1.matrix*m2.matrix, m2.inverse*m1.inverse);
}

#endif


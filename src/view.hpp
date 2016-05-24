#ifndef VIEW_HPP
#define VIEW_HPP

#include "geometry.hpp"

#include <glm/vec3.hpp>

struct GLFWwindow;

class View {
public:
    View(GLFWwindow& win);
    ~View();

    void rotate(float dx, float dy);
    void update(GLFWwindow& win);

private:
    int _width, _height;
    glm::vec3 _viewSph;
    unsigned int _frameUniformBufferID;
    Geometry::FrameUniforms _frameUniforms;
};

#endif


#include "view.hpp"

#include <glm/gtc/constants.hpp>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

// create and initialize view
View::View(GLFWwindow& win)
: _viewSph(0.f, -1.4f, 500.f) {
    // create uniform buffer objects
    glGenBuffers(1, &_frameUniformBufferID);
    glBindBuffer(GL_UNIFORM_BUFFER, _frameUniformBufferID);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(Geometry::FrameUniforms), 0,
                 GL_STREAM_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, Geometry::FRAME_UNIFORMS,
                     _frameUniformBufferID);

    // tell OpenGL to handle overlapping surfaces for this view
    glEnable(GL_DEPTH_TEST);

    // initialize scene data
    update(win);
}

View::~View() {
    glDeleteBuffers(1, &_frameUniformBufferID);
}

void View::rotate(float dx, float dy) {
    _viewSph.x += (glm::pi<float>()*dx) / static_cast<float>(_width);
    _viewSph.y += (0.5f*glm::pi<float>()*dy) / static_cast<float>(_height);
}

// New view, pointing to origin, at specified angle
void View::update(GLFWwindow& win) {
    // set viewport from window dimensions
    glfwGetFramebufferSize(&win, &_width, &_height);
    glViewport(0, 0, _width, _height);

    // adjust 3D projection into this window
    _frameUniforms.projMats = Xform::perspective(glm::pi<float>()/4.f,
        static_cast<float>(_width)/static_cast<float>(_height), 1.f, 10000.f);

    // update view matrix
    _frameUniforms.viewMats =
        Xform::translate(glm::vec3(0.f, 0.f, -_viewSph.z)) *
        Xform::xrotate(_viewSph.x) * Xform::zrotate(_viewSph.y);

    // update shader data
    glBindBuffer(GL_UNIFORM_BUFFER, _frameUniformBufferID);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Geometry::FrameUniforms),
                    &_frameUniforms);
}


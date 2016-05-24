#include "input.hpp"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

Input::Input()
: _button(-1),
  _prevButton(-1),
  _moveRate(0.f, 0.f, 0.f),
  _updateTime(glfwGetTime()),
  _wireframe(false) {
}

void Input::mousePress(GLFWwindow& win, int button, int action) {
    switch(action) {
    case GLFW_PRESS:
        glfwSetInputMode(&win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        _button = button;
        break;

    case GLFW_RELEASE:
        glfwSetInputMode(&win, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        _button = -1;
        break;
    }
}

void Input::mouseMove(View& view, GLFWwindow& win, double x, double y) {
    if (_button == GLFW_MOUSE_BUTTON_LEFT && _button == _prevButton) {
        view.rotate(x-_prevX, y-_prevY);
        view.update(win);
    }

    _prevButton = _button;
    _prevX = x;
    _prevY = y;
}

void Input::keyPress(Geometry& geom, GLFWwindow& win, int key, int action,
                     int mods) {
    switch(key) {
    case 'A':
        _moveRate.x = (action==GLFW_RELEASE) ? 0.f : -20.f;
        _updateTime = glfwGetTime();
        break;

    case 'D':
        _moveRate.x = (action==GLFW_RELEASE) ? 0.f :  20.f;
        _updateTime = glfwGetTime();
        break;

    case 'W':
        _moveRate.y = (action==GLFW_RELEASE) ? 0.f :  20.f;
        _updateTime = glfwGetTime();
        break;

    case 'S':
        _moveRate.y = (action==GLFW_RELEASE) ? 0.f : -20.f;
        _updateTime = glfwGetTime();
        break;

    case 'R':
        if (action != GLFW_RELEASE) break;
        geom.updateShaders();
        break;

    case 'L':
        if (action != GLFW_RELEASE) break;
        _wireframe = !_wireframe;
        glPolygonMode(GL_FRONT_AND_BACK, _wireframe ? GL_LINE : GL_FILL);
        break;

    case GLFW_KEY_ESCAPE:
        glfwSetWindowShouldClose(&win, action==GLFW_RELEASE);
        break;
    };
}


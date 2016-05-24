#ifndef INPUT_HPP
#define INPUT_HPP

#include "geometry.hpp"
#include "view.hpp"

#include <glm/vec3.hpp>

struct GLFWwindow;

class Input {
public:
    Input();

    void mousePress(GLFWwindow& win, int button, int action);
    void mouseMove(View& view, GLFWwindow& win, double x, double y);
    void keyPress(Geometry& geom, GLFWwindow& win, int key, int action,
                  int mods);

private:
    int _button, _prevButton;
    double _prevX, _prevY;
    glm::vec3 _moveRate;
    double _updateTime;
    bool _wireframe;
};

#endif


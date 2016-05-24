#include "config.h"
#include "input.hpp"
#include "view.hpp"
#include "sphere.hpp"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <cstdlib>
#include <cstdio>

struct AppContext {
    Geometry geom;
    Input input;
    View view;

    AppContext(GLFWwindow& win)
    : view(win) {
        glfwSetWindowUserPointer(&win, this);
    }


    static void glfwError(int error, const char* description) {
        std::fprintf(stderr, "GLFW error: %s\n", description);
    }

    static void reshape(GLFWwindow* win, int width, int height) {
        AppContext* ctx = reinterpret_cast<AppContext*>(glfwGetWindowUserPointer(win));
        ctx->view.update(*win);
    }

    static void mousePress(GLFWwindow* win, int button, int action, int mods) {
        AppContext* ctx = reinterpret_cast<AppContext*>(glfwGetWindowUserPointer(win));
        ctx->input.mousePress(*win, button, action);
    }

    static void mouseMove(GLFWwindow* win, double x, double y) {
        AppContext* ctx = reinterpret_cast<AppContext*>(glfwGetWindowUserPointer(win));
        ctx->input.mouseMove(ctx->view, *win, x, y);
    }

    static void keyPress(GLFWwindow* win, int key, int, int action, int mods) {
        AppContext* ctx = reinterpret_cast<AppContext*>(glfwGetWindowUserPointer(win));
        ctx->input.keyPress(ctx->geom, *win, key, action, mods);
    }

    static void debugOutput(GLuint source, GLuint type, GLuint id, GLuint severity,
                            GLint length, const char* message, const void*) {
        static const char* SOURCE[] = {
            "API", "WINDOW SYSTEM", "SHADER COMPILER", "THIRD PARTY", "APPLICATION", "OTHER"
        };
        static const char* SEVERITY[] = {
            "HIGH", "MEDIUM", "LOW", "NOTIFICATION"
        };
        static const char* TYPE[] = {
            "ERROR", "DEPRECATED", "UNDEFINED", "PORTABILITY", "PERFORMANCE", "OTHER"
        };

        std::fprintf(stderr, "%s [%s] : %s (%s)\n",
            TYPE[type - GL_DEBUG_TYPE_ERROR],
            SEVERITY[
                severity != GL_DEBUG_SEVERITY_NOTIFICATION ?
                severity  - GL_DEBUG_SEVERITY_HIGH : 3
            ],
            message,
            SOURCE[source - GL_DEBUG_SOURCE_API]
        );
    }

    static GLFWwindow* initGLFW() {
        glfwSetErrorCallback(AppContext::glfwError);
        if (!glfwInit()) abort();

        glfwWindowHint(GLFW_SAMPLES, 4);
        glfwWindowHint(GLFW_SRGB_CAPABLE, GL_TRUE);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifndef NDEBUG
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
#endif

        GLFWwindow* win = glfwCreateWindow(1280, 720, "test", NULL, NULL);
        if (!win) abort();

        glfwMakeContextCurrent(win);
        glewExperimental = GL_TRUE;
        if (glewInit() != GLEW_OK) abort();

        std::printf("OpenGL %s/GLSL %s\n", glGetString(GL_VERSION),
                    glGetString(GL_SHADING_LANGUAGE_VERSION));

#ifndef NDEBUG
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(AppContext::debugOutput, NULL);
#endif

        glfwSetFramebufferSizeCallback(win, AppContext::reshape);
        glfwSetKeyCallback(win, AppContext::keyPress);
        glfwSetMouseButtonCallback(win, AppContext::mousePress);
        glfwSetCursorPosCallback(win, AppContext::mouseMove);

        glClearColor(1.f, 1.f, 1.f, 1.f);
        return win;
    }
};

int main(void) {
    GLFWwindow* win = AppContext::initGLFW();
    AppContext ctx(*win);

    // create stuff to draw
    Sphere sph0(ctx.geom, 50, glm::vec3(0.f, 0.f, 0.f));
    ctx.geom.finalizeDrawData();

    while (!glfwWindowShouldClose(win)) {
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        ctx.geom.draw();

        glfwSwapBuffers(win);
        glfwPollEvents();
    }

    glfwDestroyWindow(win);
    glfwTerminate();
}


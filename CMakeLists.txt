cmake_minimum_required(VERSION 2.8.11)
project(gl-geometry-modperf C CXX)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(glew)
include(glfw)
include(glm)

find_package(OpenGL REQUIRED)
set(OPENGL_INCLUDE_DIR ${OPENGL_INCLUDE_DIR} CACHE STRING "")
set(OPENGL_LIBRARIES ${OPENGL_LIBRARIES} CACHE STRING "")

add_subdirectory(src)

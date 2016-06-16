cmake_minimum_required(VERSION 2.8.11)
project(gl-geometry-modperf C CXX)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(turf)
include(glew)
include(glfw)
include(glm)

find_package(OpenGL REQUIRED)
set(OPENGL_INCLUDE_DIR ${OPENGL_INCLUDE_DIR} CACHE STRING "")
set(OPENGL_LIBRARIES ${OPENGL_LIBRARIES} CACHE STRING "")

add_subdirectory(src)

set_source_files_properties("${PROJECT_SOURCE_DIR}/tags"
    PROPERTIES GENERATED TRUE)
add_custom_target(tags ALL
    COMMAND ctags -R
        "${PROJECT_SOURCE_DIR}/src/*"
        "${PROJECT_BINARY_DIR}/turf/src/turf/*"
        "${PROJECT_BINARY_DIR}/glew/src/glew/*"
        "${PROJECT_BINARY_DIR}/glfw/src/glfw/*"
        "${PROJECT_BINARY_DIR}/glm/src/glm/*"
    WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
)

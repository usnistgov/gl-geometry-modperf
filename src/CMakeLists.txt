set(PROJECT_DATA_DIR "${PROJECT_SOURCE_DIR}/data")
configure_file(config.h.in config.h)

include_directories(${CMAKE_CURRENT_BINARY_DIR} ${TURF_INCLUDE_DIR}
    ${GLM_INCLUDE_DIRS} ${GLFW_INCLUDE_DIR} ${GLEW_INCLUDE_DIRS})

set(SOURCES
    geometry.cpp
    input.cpp
    view.cpp
    xform.cpp
    shader.cpp
    sphere.cpp
    timer.cpp
)

add_library(framework ${SOURCES})
add_dependencies(framework glm glfw glew)
target_link_libraries(framework ${GLFW_LIBRARIES} ${GLEW_LIBRARIES} ${OPENGL_LIBRARIES})

add_executable(w w.cpp)
target_link_libraries(w framework)

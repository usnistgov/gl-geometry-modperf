set(PROJECT_DATA_DIR "${PROJECT_SOURCE_DIR}/data")
configure_file(config.h.in config.h)

include_directories(${CMAKE_CURRENT_BINARY_DIR}
    ${GLM_INCLUDE_DIRS} ${GLFW_INCLUDE_DIR} ${GLEW_INCLUDE_DIRS})

add_library(framework geometry.cpp input.cpp view.cpp xform.cpp shader.cpp sphere.cpp)

add_executable(w w.cpp)
add_dependencies(w glm glfw glew)
target_link_libraries(w framework ${GLFW_LIBRARIES} ${GLEW_LIBRARIES} ${OPENGL_LIBRARIES})

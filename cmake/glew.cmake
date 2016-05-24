include(ExternalProject)
find_package(glew 1.13.0 QUIET)

if(GLEW_FOUND)
    message(STATUS "Found GLEW")
else()
    message(STATUS "GLEW not found - will build from source")
    file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/glew-CMakeLists.txt
        "cmake_minimum_required(VERSION @CMAKE_VERSION@)\n\n"
        "file(APPEND auto/blacklist \"NV/draw_vulkan_image\")\n"
        "execute_process(\n"
        "   WORKING_DIRECTORY \${CMAKE_CURRENT_SOURCE_DIR}/auto\n"
        "   COMMAND \${CMAKE_MAKE_PROGRAM}\n"
        ")\n\n"
        "add_subdirectory(build/cmake)\n"
    )

    ExternalProject_Add(glew PREFIX glew
        GIT_REPOSITORY https://github.com/nigels-com/glew.git
        GIT_TAG glew-1.13.0

        UPDATE_COMMAND ""
        PATCH_COMMAND
            ${CMAKE_COMMAND} -E copy
                ${CMAKE_CURRENT_BINARY_DIR}/glew-CMakeLists.txt
                <SOURCE_DIR>/CMakeLists.txt

        CMAKE_ARGS
            "-DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>"
            "-DCMAKE_BUILD_TYPE=Release"

        LOG_DOWNLOAD 1 LOG_UPDATE 1 LOG_CONFIGURE 1 LOG_BUILD 1 LOG_INSTALL 1
    )

    ExternalProject_Get_Property(glew INSTALL_DIR)
    set(GLEW_INCLUDE_DIRS ${INSTALL_DIR}/include)
    set(GLEW_LIBRARIES
        ${INSTALL_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}GLEW${CMAKE_STATIC_LIBRARY_SUFFIX})

endif()

set(GLEW_INCLUDE_DIRS ${GLEW_INCLUDE_DIRS} CACHE STRING "")
set(GLEW_LIBRARIES ${GLEW_LIBRARIES} CACHE STRING "")

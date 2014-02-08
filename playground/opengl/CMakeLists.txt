project(opengl_fw)

cmake_minimum_required(VERSION 2.8.5)

find_package(PkgConfig)

if (APPLE)
    pkg_check_modules(deps REQUIRED glfw3)
    include_directories(/System/Libraries/Frameworks
                        ${deps_INCLUDE_DIRS})
    link_directories(${deps_LIBRARY_DIRS})
    find_library(OpenGL_LIBRARY OpenGL)
    list(APPEND DEP_LIBS "-framework=${OpenGL_LIBRARY}" ${deps_LIBRARIES})
else (APPLE)
    pkg_check_modules(glfw REQUIRED glfw3)
    find_package(X11 REQUIRED)
    include_directories(${glfw_INCLUDE_DIRS})
    link_directories(${glfw_LIBRARY_DIRS})
    list(APPEND DEP_LIBS X11 GL ${glfw_LIBRARIES})
endif (APPLE)

set(TARGET_NAME ${PROJECT_NAME})

set(SRCS test.cc)

add_executable(${TARGET_NAME} ${SRCS})
target_link_libraries(${TARGET_NAME} ${DEP_LIBS})

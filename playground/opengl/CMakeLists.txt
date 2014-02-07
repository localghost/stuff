project(opengl_fw)

cmake_minimum_required(VERSION 2.8.5)

find_package(PkgConfig)
find_package(X11 REQUIRED)

pkg_check_modules(glfw REQUIRED glfw3)

set(TARGET_NAME ${PROJECT_NAME})

include_directories(${glfw_INCLUDE_DIRS})
link_directories(${glfw_LIBRARY_DIRS})

set(SRCS test.cc)

add_executable(${TARGET_NAME} ${SRCS})
target_link_libraries(${TARGET_NAME} X11 GL ${glfw_LIBRARIES})

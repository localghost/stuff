project(Variant)

cmake_minimum_required(VERSION 2.8)

set(SRCS main.cpp)

set(TARGET_NAME "variant")

add_definitions("--std=c++0x")

add_executable(${TARGET_NAME} ${SRCS})

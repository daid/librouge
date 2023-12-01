cmake_minimum_required(VERSION 3.12)
project(rougetemplate)

file(GLOB_RECURSE SOURCES src/*.cpp src/*.h)

add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} PUBLIC librouge-core librouge-ui)

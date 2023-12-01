cmake_minimum_required(VERSION 3.12)
project(librouge)

file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp ${CMAKE_CURRENT_LIST_DIR}/include/*.h)
add_library(librouge-core OBJECT ${SOURCES})

target_link_libraries(librouge-core PUBLIC glm::glm SDL2::SDL2)
target_include_directories(librouge-core PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)
target_compile_options(librouge-core PUBLIC -Wall -Wextra)
target_compile_features(librouge-core PUBLIC cxx_std_17)


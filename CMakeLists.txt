cmake_minimum_required(VERSION 3.12)
project(librouge)

file(GLOB_RECURSE SOURCES src/*.cpp include/*.h)
add_library(librouge OBJECT ${SOURCES})

include("cmake/glm.cmake")

if (EMSCRIPTEN)
    add_library(SDL2 INTERFACE)
    target_compile_options(SDL2 INTERFACE -sUSE_SDL=2 -sALLOW_MEMORY_GROWTH=1 -fno-exceptions)
    target_link_options(SDL2 INTERFACE -sUSE_SDL=2 -sALLOW_MEMORY_GROWTH=1 -fno-exceptions)
    add_library(SDL2::SDL2 ALIAS SDL2)
else()
    find_package(SDL2 REQUIRED)
endif()

target_link_libraries(librouge PUBLIC glm::glm SDL2::SDL2)
target_include_directories(librouge PUBLIC include)
target_compile_options(librouge PUBLIC -Wall -Wextra)
target_compile_features(librouge PUBLIC cxx_std_17)

if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_SOURCE_DIR})
    add_subdirectory(template)
endif()
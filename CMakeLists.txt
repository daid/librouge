cmake_minimum_required(VERSION 3.12)
project(librouge)

file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp ${CMAKE_CURRENT_LIST_DIR}/include/*.h)
add_library(librouge OBJECT ${SOURCES})

include("${CMAKE_CURRENT_LIST_DIR}/cmake/glm.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/cmake/sdl2.cmake")


target_link_libraries(librouge PUBLIC glm::glm SDL2::SDL2)
target_include_directories(librouge PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)
target_compile_options(librouge PUBLIC -Wall -Wextra)
target_compile_features(librouge PUBLIC cxx_std_17)

if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_LIST_DIR})
    add_subdirectory(template)
endif()
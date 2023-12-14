cmake_minimum_required(VERSION 3.12)
project(librouge)

include("${CMAKE_CURRENT_LIST_DIR}/cmake/glm.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/cmake/sdl2.cmake")

add_subdirectory(core)
add_subdirectory(ui)
add_library(librouge ALIAS librouge-core)

if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_LIST_DIR})
    add_subdirectory(template)
endif()

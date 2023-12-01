cmake_minimum_required(VERSION 3.12)
project(librouge)

file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp ${CMAKE_CURRENT_LIST_DIR}/include/*.h)
add_library(librouge-ui OBJECT ${SOURCES})

target_link_libraries(librouge-ui PUBLIC librouge-core)
target_include_directories(librouge-ui PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)

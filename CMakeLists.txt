cmake_minimum_required(VERSION 3.12)
project(librouge)

file(GLOB_RECURSE SOURCES src/*.cpp include/*.h)
add_library(librouge OBJECT ${SOURCES})

function(get_git_library NAME URL TAG)
    if(NOT IS_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/extlib/${NAME}/)
        execute_process(COMMAND git -c advice.detachedHead=false clone ${URL} ${CMAKE_CURRENT_BINARY_DIR}/extlib/${NAME}/ --branch ${TAG} --depth 1)
    endif()
    if(EXISTS ${CMAKE_CURRENT_BINARY_DIR}/extlib/${NAME}/CMakeLists.txt)
        add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/extlib/${NAME}/ ${CMAKE_CURRENT_BINARY_DIR}/extlib_bin/${NAME}/)
    endif()
endfunction()

# Get glm, which is header only, and can directly be used
get_git_library(glm https://github.com/g-truc/glm.git 0.9.9.8)

find_package(SDL2 REQUIRED)

target_link_libraries(librouge PUBLIC glm::glm SDL2::SDL2)
target_include_directories(librouge PUBLIC include)
target_compile_options(librouge PUBLIC -Wall -Wextra)

if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_SOURCE_DIR})
    add_subdirectory(template)
endif()
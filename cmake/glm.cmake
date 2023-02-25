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

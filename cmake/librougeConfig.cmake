if(NOT TARGET librouge-core)
    add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/../" ${CMAKE_CURRENT_BINARY_DIR}/librouge/)
endif()

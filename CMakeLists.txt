cmake_minimum_required(VERSION 3.0.0)
project(k_tree VERSION 0.1.0)

option(BUILD_DOC "Build documentation" ON)
option(ASAN "address sanitizer" OFF)

if(BUILD_DOC)
    find_package(Doxygen)
    if (DOXYGEN_FOUND)
        # set input and output files
        set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile.in)
        set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

        # request to configure the file
        configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)

        # note the option ALL which allows to build the docs together with the application
        add_custom_target(doc_doxygen ALL
            COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            COMMENT "Generating API documentation with Doxygen"
            VERBATIM)
    else ()
        message("Doxygen need to be installed to generate the doxygen documentation")
    endif ()
endif()

if(CMAKE_BUILD_TYPE MATCHES Debug)
    if(ASAN)
        message("Debug build with asan")
        set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-omit-frame-pointer -fsanitize=address")
        set (CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS} -fno-omit-frame-pointer -fsanitize=address")
    endif()
endif()

include(CTest)
enable_testing()
include_directories(include/k_tree)
include_directories(include/list)
include_directories(include/graph)
include_directories(tests)

add_executable(tree_random_test         tests/k_tree/random_test.cpp)
add_executable(tree_copy_move_test      tests/k_tree/copy_move_test.cpp)
add_executable(tree_clear_test          tests/k_tree/clear_test.cpp)
add_executable(tree_breadth_wise_test   tests/k_tree/breadth_wise_test.cpp)

add_executable(list_random_test         tests/list/random_test.cpp)
add_executable(list_copy_move_test      tests/list/copy_move_test.cpp)

#add_executable(graph_test               tests/graph/test.cpp)

add_test(tree_random_test       tree_random_test)
add_test(tree_copy_move_test    tree_copy_move_test)
add_test(tree_clear_test        tree_clear_test)
add_test(tree_breadth_wise_test tree_breadth_wise_test)

add_test(list_random_test       list_random_test)
add_test(list_copy_move_test    list_copy_move_test)

#add_test(graph_test             graph_test)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

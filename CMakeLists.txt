cmake_minimum_required (VERSION 3.6)
project (PEA_project3_TSP_genetic CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-O3")


#######################################################
#                                                     #
# Add new directories with *.cpp files to this list   #
# All source code should be compiled in build process #
#                                                     #
#######################################################
set(SOURCE_DIRECTORIES
        Algorithms/
        Application/
        Graph/
        Miscellanous/
   )


foreach(SOURCE_DIRECTORY ${SOURCE_DIRECTORIES})

    set(${SOURCE_DIRECTORY}_REGEX ${SOURCE_DIRECTORY}*.cpp)
    file(GLOB_RECURSE ${SOURCE_DIRECTORY}_FILES ${${SOURCE_DIRECTORY}_REGEX})
    list(APPEND SOURCE_FILES ${${SOURCE_DIRECTORY}_FILES})

endforeach()


list(APPEND SOURCE_FILES main.cpp)
add_executable(bin ${SOURCE_FILES})
target_include_directories(bin PRIVATE ${CMAKE_SOURCE_DIR})

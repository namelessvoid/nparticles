################################################################################
# Copyright (C) 2015 Simon Kerler                                              #
#                                                                              #
# This file is part of the "Nameless Particle Engine".                         #
# For conditions of distribution and use, see the copyright notice you should  #
# have recievied with this software. I not, see:                               #
# http://opensource.org/licenses/Zlib                                          #
################################################################################

project(np-engine)
cmake_minimum_required(VERSION 2.8)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
set(CMAKE_CXX_FLAGS "-std=c++11 -Wall")
set(CMAKE_BUILD_TYPE Debug)

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/include)

add_subdirectory(src)
add_subdirectory(samples)

# IDE stuff: Show headers and resources in IDEs like QtCreator or Visual Studio
file(GLOB_RECURSE ide_files RELATIVE ${np-engine_SOURCE_DIR}
        # glob expressions
        ${np-engine_SOURCE_DIR}/include/*
        ${np-engine_SOURCE_DIR}/res/*
)
add_custom_target(np-engine-ide-target SOURCES ${ide_files})

################################################################################
# Copyright (C) 2015 Simon Kerler                                              #
#                                                                              #
# This file is part of the "Nameless Particle Engine".                         #
# For conditions of distribution and use, see the copyright notice you should  #
# have recievied with this software. I not, see:                               #
# http://opensource.org/licenses/Zlib                                          #
################################################################################

find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)

find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)

find_package(Boost 1.50 COMPONENTS system filesystem REQUIRED)
#find_package(GLM REQUIRED)

add_library(npengine
    gpusystem.cpp
    rendersystem.cpp
    engine.cpp
    buffer.cpp
    atomiccounterbuffer.cpp
    fileutils.cpp
    particlesystem.cpp
    gpuprogramservice.cpp
    shaderprogram.cpp
    renderprogram.cpp
    computeprogram.cpp
    mesh.cpp
    meshmanager.cpp
    material.cpp
    materialmanager.cpp
    camera.cpp
    computesystem.cpp
    action.cpp
    glutils.cpp
    gpuclock.cpp
    cpuclock.cpp
)

target_link_libraries(npengine ${GLEW_LIBRARIES} ${OPENGL_LIBRARIES} ${GLFW_LIBRARIES} ${Boost_LIBRARIES})

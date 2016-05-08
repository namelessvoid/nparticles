################################################################################
# Copyright (C) 2015 Simon Kerler                                              #
#                                                                              #
# This file is part of the "Nameless Particle Engine".                         #
# For conditions of distribution and use, see the copyright notice you should  #
# have recievied with this software. I not, see:                               #
# http://opensource.org/licenses/Zlib                                          #
################################################################################

add_executable(fullexample fullexample.cpp)
target_link_libraries(fullexample npengine)

add_executable(glutils glutils.cpp)
target_link_libraries(glutils npengine)

add_executable(defaultresources defaultresources.cpp)
target_link_libraries(defaultresources npengine)

add_executable(buffermapping buffermapping.cpp)
target_link_libraries(buffermapping npengine)

add_executable(gravity gravity.cpp)
target_link_libraries(gravity npengine)

add_executable(solarsystem solarsystem.cpp)
target_link_libraries(solarsystem npengine)

add_executable(icosahedron icosahedron.cpp)
target_link_libraries(icosahedron npengine)

add_executable(renderbenchmark renderbenchmark.cpp)
target_link_libraries(renderbenchmark npengine)

add_executable(particleevents particleevents.cpp)
target_link_libraries(particleevents npengine)

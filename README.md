# Nameless Particle Engine

The Nameless Particle Engine (NParticles) is a particle and n-body simulation engine with GPGPU support. It is written in C++ and uses OpenGL 4.3 as render and GPGPU framework.

**Table of contents**
<!-- MarkdownTOC depth=3 -->

- Compilation
	- Dependencies
	- Compile library with cmake
	- Compile samples manually
- Sample applications
- API
- Licensing

<!-- /MarkdownTOC -->

## Compilation

### Dependencies

Following libraries have to be installed to compile the engine:

- OpenGL (>= 4.3)
- GLEW (>= 1.11.0)
- GLFW (>= 3.0.4)
- GLM (>= 0.9.6)
- Boost filesystem (>= 1.50)
- pkgconfig (to find GLFW3)

The version numbers are those I used for compilation. Older versions may work as well (except for OpenGL!) but are not tested yet.

Some Linux distributions do not install the FindGLM.cmake module when installing the GLM library. This file can be found [here](https://github.com/g-truc/glm/blob/1a4bc0d8a55123b06fdb7e866da9a8b9172446a4/util/FindGLM.cmake). Download this file into a directory (e.g. `cmake/Modules` inside the root directory of this engine). To make it available for cmake, add following line to the `CMakeLists.txt` inside the root directory:

    SET(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/Modules ${CMAKE_MODULE_PATH})

**Note:** To run the engine, the OpenGL extension `GL_ARB_shading_language_include` is required! Future releases may get rid of this extension since it may not be supported by all drivers.



### Compile library with cmake

Just create a build directory: `mkdir build`
Change to this directory, execute cmake and run make: `cd build && cmake .. && make`

This creates a static library placed in `lib/` inside the root directory. This library can be used to be linked again applications using the engine.

The samples shipped with the engine are stored in the `bin` directory located in the root directory of the engine. To execute these samples, they have to be called from inside the `bin` directory.



### Compile samples manually

To manually compile one of the samples, following command can be used (from within the `build` directory):

`$ g++ ../samples/solarsystem.cpp -o solarsystem -std=c++11 ../src/*.cpp -I../include/ -lGLEW -lGL -lglfw -lboost_filesystem -lboost_system`



## Sample applications

The documentation of the sample applications can be found in `doc/samples.md`.


## API

NParticles ships with a *doxygen* API. Just run `doxygen` in the engines root folder.

A class diagramm (full and simplified version) are located in `doc/diagrams`.
You need [Umlet](http://www.umlet.com/) to view them.

## Licensing

NParticles is published under the zlib license. The detailed license text is provided in `license.txt`.

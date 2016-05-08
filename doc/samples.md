# NParticles Sample applications

**Table of contents**

<!-- MarkdownTOC -->

- Standard controls
- Gravity
- Solar system
- Render benchmark
- Buffer mapping
- Default resource
- Full example
- glutils
- Icosahedron
- Particle events

<!-- /MarkdownTOC -->

## Standard controls

Each simulation uses the engine built-in standard controls which are

- wasd: foreward, strafe left, backward, strafe right
- xy: rotate counter clockwise / clockwise
- qe: descend / ascend
- LMB pressed + mouse movement: rotate camera



## Gravity

This sample contains a particle system with gravitation based particles as well as an update benchmark. There are different integration types and optimizations provided to udpate the particles. By passing command line switches, the desired type can be selected:

- `-euler-no-shared`:          Improved euler integration without shared memory optimization.
- `-euler-shared`:             Improved euler integration with shared memory optimization.
- `-verlet-no-shared`:         Verlet integration withouth shared memory optimization.
- `-verlet-shared`:            Verlet integration with shared memory optimization.
- `-verlet-shared-buffering`:  Verlet integration with shared memory optimization and client-side double buffering.

### Interactive simulation

Controls:

- Space to toggle pause
- nm to reduce / increase time step
- c toggles quad / circle shaped particles

Color modes selected through number keys:

- 1: velocity vector
- 2: speed (length of velocity vector)
- 3: distance to (0, 0, 0)
- 4: plain yellow

The FPS per frame are logged to standard output.

### Benchmark

The benchmark subsequetially updates particle systems with different particle counts. Each configuration is updated 100 times to increase accuracy. For integration, the integration method and optimisation technique selected on command line is used (see above). For each update call, set up, update and clean up processes are profiled. The eastimated GFLOPS are calculated as well.



## Solar system

This simulation simulates the movement of the 8 planets of our solar system and the dwarf planet Pluto. It uses scientific data to initialise planets. These are rendered as tessellated icosahedra in wireframe mode.

Controls:

- Space: toggle pause mode
- v: toggle v-sync
- t: draw trajectories (camera should not be moved in this mode to avoid visual artifacts!)



## Render benchmark

This application provides a utility to benchmark the render performance of the engine.

It subsequentially renders different amounts of particles. Particles are represented by untessellated icosahedra. Each configuration is rendered 1000 times to increase accuracy.

Times for set up, render and clean up processes are profiled by using the particle systems render events.



## Buffer mapping

This example demonstrates how buffer mapping is used. It creates a particle system with 100 particles rendered as red dots. The positions of these particles are initialised by using buffer mapping. To make the result visible, the camera has to be moved backwards.



## Default resource

This application was introduced to verify the implementation of the default resoruces. A particle system with one particle is created that uses default resources for mesh and material. Result is a single ret dot that is rendered at the origin of the simulation (camera has to be moved backwards to make it visible).



## Full example

This (little chaotic) sample application was used to verify first implementations of the engine and is not meant to be meaningful.

It creates a particle system of 100 particles which are rendered as triangles. To update shaders move them upwards and to the right respectively.

Several different aspects are covered by this application:

- Test the action list by executing two update shaders sequentially.
- Setting up custom uniforms and buffers for rendering.
- Create a custom mesh (a triangle).
- Pass a custom rotation matrix to the render process.


## glutils

This is a very basic application to test if glutils::getGlTypeInfo() was implemented correctly.


## Icosahedron

This sample renders a yellow icosahedron which is provided as a standard mesh by the MeshManager. The mesh is rendered in plain yellow since no lighting is applied.


## Particle events

 This sample demonstrates how particle system events are used. Callbacks are registered to all 4 events which print a message to stdout.

Particles are not updated but rendered fixed at the origin of the simulation.

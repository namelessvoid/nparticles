/**
 * Basic vertex shader inputs provided by NParticles.
 *
 * This header contains the basic vertex shader inputs provided by the NParticles
 * engine. These are.
 */

#ifndef GRAVITY_BUFFER_GLSL
#define GRAVITY_BUFFER_GLSL

/**
 * Definition of inputs (uniforms and buffers) to the shaders of
 * the gravity application.
 */

/**
 * Definition of the structure used for the position buffer.
 */
struct PositionStruct
{
    vec3 position;
    float mass;
};

/**
 * Definition of the shader storage block storing positions and masses.
 */
layout (binding = 0) buffer ParticlePositions
{
    PositionStruct positions[];
};

/**
 * Definition of the shader storage block storing velcoties (Euler integration) or
 * the old positions (Verlet integration).
 */
layout (binding = 1) buffer ParticleProperties
{
    vec4 properties[];
};

/**
 * The number of particles in the simulation.
 */
uniform uint  particleCount;

/**
 * The size of the time step.
 */
uniform float timeStep;

/**
 * Flag set to true, if Verlet integration is used. Otherwise it is false.
 */
uniform bool usingVerlet;
#endif

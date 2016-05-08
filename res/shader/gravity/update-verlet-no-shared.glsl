/**
 * Basic vertex shader inputs provided by NParticles.
 *
 * This header contains the basic vertex shader inputs provided by the NParticles
 * engine. These are.
 */

#version 430

#extension GL_ARB_shading_language_include : enable

layout(local_size_x = 1024) in;

#include </np/globalinvocationindex.glsl>
#include </np/gravity-utils.glsl>
#include </gravity/inputs.glsl>

/*
 * Update shader using Verlet integration. No shared memory is used.
 */
void main()
{
    uint globalInvocationIndex = npGetGlobalInvocationIndex();

    // Return if this shader invocation was issued to fill up last work group
    // when particleCount not multiple of 1024
    if(globalInvocationIndex >= particleCount)
        return;

    vec3 currentPosition = positions[globalInvocationIndex].position;

    vec3 acceleration = vec3(0.0, 0.0, 0.0);

    float softeningFactor = 0.1;
    for(int i = 0; i < particleCount; ++i)
    {
        acceleration += npCalcAcceleration(currentPosition, positions[i].position, positions[i].mass, softeningFactor);
    }

    vec3 newPosition = 2 * currentPosition - properties[globalInvocationIndex].xyz + acceleration * timeStep * timeStep;

    positions[globalInvocationIndex].position = newPosition;
    properties[globalInvocationIndex].xyz = currentPosition;
}

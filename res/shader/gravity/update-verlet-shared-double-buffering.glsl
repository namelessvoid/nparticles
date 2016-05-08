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

shared PositionStruct sharedPositions[gl_WorkGroupSize.x];

/**
 * Update shader using verlet integration and shared memory.
 *
 * The old and new position buffers have to be swapped by the application!
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

    for(uint i = 0, currentTile = 0; i < particleCount; i += gl_WorkGroupSize.x, ++currentTile)
    {
        uint tileBegin = currentTile * gl_WorkGroupSize.x;

        if((tileBegin + gl_LocalInvocationIndex) < particleCount)
        {
            sharedPositions[gl_LocalInvocationIndex] = positions[tileBegin + gl_LocalInvocationIndex];
        }

        barrier();

        for(int p = 0; p < gl_WorkGroupSize.x && (tileBegin + p) < particleCount; ++p)
        {
            acceleration += npCalcAcceleration(currentPosition, sharedPositions[p].position, sharedPositions[p].mass, softeningFactor);
        }

        barrier();
    }

    // 3 + 3 + 3 + 3 * 2 = 15
    vec3 newPosition = 2 * currentPosition - properties[globalInvocationIndex].xyz + acceleration * timeStep * timeStep;

    properties[globalInvocationIndex].xyz = newPosition;
}

/**
 * Basic vertex shader inputs provided by NParticles.
 *
 * This header contains the basic vertex shader inputs provided by the NParticles
 * engine. These are.
 */

#version 430

#extension GL_ARB_shading_language_include : enable

layout (local_size_x = 1024) in;

#include </np/globalinvocationindex.glsl>
#include </np/gravity-utils.glsl>

#include </gravity/inputs.glsl>

shared PositionStruct shPositions[gl_WorkGroupSize.x];

/**
 * Update shader using improved Euler itnegration and shared memory.
 */
void main()
{
    uint globalInvocationIndex = npGetGlobalInvocationIndex();

    // Return if this shader invocation was issued to fill up last work group
    // when particleCount not multiple of 1024
    if(globalInvocationIndex >= particleCount)
        return;

    vec3 position = positions[globalInvocationIndex].position;
    vec3 oldVelocity = properties[globalInvocationIndex].xyz;

    vec3 acceleration = vec3(0.0, 0.0, 0.0);

    float softeningFactor = 0.1;
    for(uint i = 0, currentTile = 0; i < particleCount; i += gl_WorkGroupSize.x, ++currentTile)
    {
        uint tileBegin = currentTile * gl_WorkGroupSize.x;
        // Bounds check so no trash is stored in shared variable if last tile is not fully populated,
        // i.e. particleCount is not a multiple of 1024.
        if((tileBegin + gl_LocalInvocationIndex) < particleCount)
            // Populate shared variable
            shPositions[gl_LocalInvocationIndex] = positions[tileBegin + gl_LocalInvocationIndex];
        barrier();

        // For all particles in the tile, update acceleartion. Check that access to last tile does
        // not exceed the particle count.
        for(uint p = 0; p < gl_WorkGroupSize.x && (tileBegin + p) < particleCount; ++p)
        {
            // Sum: 3 FLOP per particle => particleCount * 3 FLOPS
            // npCalcAcceleration: 19 FLOPS per calculation => particleCount * 19 FLOPS
            acceleration += npCalcAcceleration(position, shPositions[p].position, shPositions[p].mass, softeningFactor);
        }

        barrier();
    }

    // 2 * 3 = 6 FLOPS
    vec3 newVelocity = oldVelocity + acceleration * timeStep;

    // 4 * 3 = 12 FLOPS
    position += 0.5 * (oldVelocity + newVelocity) * timeStep;

    positions[globalInvocationIndex].position = position;
    properties[globalInvocationIndex].xyz = newVelocity;
}

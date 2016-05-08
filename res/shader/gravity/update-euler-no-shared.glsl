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

/**
 * Update shader using improved Euler integration and no shared memory.
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
    for(uint i = 0; i < particleCount; ++i)
    {
        // Sum: 3 FLOP per particle => particleCount * 3 FLOPS
        // npCalcAcceleration: 19 FLOPS per calculation => particleCount * 19 FLOPS
        acceleration += npCalcAcceleration(position, positions[i].position, positions[i].mass, softeningFactor);
    }

    // 2 * 3 = 6 FLOPS
    vec3 newVelocity = oldVelocity + acceleration * timeStep;

    // 4 * 3 = 12 FLOPS
    position += 0.5 * (oldVelocity + newVelocity) * timeStep;

    positions[globalInvocationIndex].position = position;
    properties[globalInvocationIndex].xyz = newVelocity;
}

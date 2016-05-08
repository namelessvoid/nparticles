/**
 * Basic vertex shader inputs provided by NParticles.
 *
 * This header contains the basic vertex shader inputs provided by the NParticles
 * engine. These are.
 */

#version 430
#extension GL_ARB_shading_language_include : require

layout(local_size_x = 10) in;

#include </fullexample/buffers.glsl>

/**
 * Update shader that moves all input positions to the right.
 */
void main()
{
    float x = position[gl_GlobalInvocationID.x].x;

    x += 0.01;

    position[gl_GlobalInvocationID.x].x = x;
}

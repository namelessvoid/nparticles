/**
 * Basic vertex shader inputs provided by NParticles.
 *
 * This header contains the basic vertex shader inputs provided by the NParticles
 * engine. These are.
 */

#version 430

#extension GL_ARB_shading_language_include : require

#include </fullexample/buffers.glsl>

layout(local_size_x = 10) in;

/**
 * Update shader moving all input positions upwards.
 */
void main()
{
    position[gl_GlobalInvocationID.x].y += 0.01;
}

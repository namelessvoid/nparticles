/*
 * Copyright (C) 2015 Simon Kerler
 *
 * This file is part of the "Nameless Particle Engine".
 * For conditions of distribution and use, see the copyright notice you should
 * have recievied with this software. I not, see:
 * http://opensource.org/licenses/Zlib
 */

#version 430

#extension GL_ARB_shading_language_include : require

#include </np/vertex-inputs.glsl>

#include </solarsystem/buffers.glsl>

out vec4 v_position;

out vec3 v_particlePosition;
out float v_particleScale;

flat out vec4 v_instanceColor;

/**
 * Simple pass-through vertex shader. Vertex transformation is performed in the tessellation
 * evaulation shader.
 */
void main()
{
    v_position = vec4(np_in_position, 1.0);
    v_particlePosition = vec3(positions[gl_InstanceID].xyz);
    v_particleScale = float(velocities[gl_InstanceID].w);

    v_instanceColor = colors[gl_InstanceID];
}

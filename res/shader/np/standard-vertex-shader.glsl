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
#include </np/uniforms.glsl>

buffer Positions
{
    vec4 positions[];
};

/**
 * Standard vertex shader.
 *
 * It performs particle translation using the Positions buffer as well as
 * view projection transformation by the engine built-in view projection
 * matrix.
 */
void main()
{
    gl_Position = np_viewProjectionMatrix * vec4(np_in_position + positions[gl_InstanceID].xyz, 1.0);
}

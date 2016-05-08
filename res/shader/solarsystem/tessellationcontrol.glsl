/*
 * Copyright (C) 2015 Simon Kerler
 *
 * This file is part of the "Nameless Particle Engine".
 * For conditions of distribution and use, see the copyright notice you should
 * have recievied with this software. I not, see:
 * http://opensource.org/licenses/Zlib
 */

#version 430 core

layout (vertices = 3) out;

in vec4 v_position[];
in vec3 v_particlePosition[];
in float v_particleScale[];
flat in vec4 v_instanceColor[];

out vec3 tc_position[];
out vec3 tc_particlePosition[];
out float tc_particleScale[];
flat out vec4 tc_instanceColor[];

void main()
{
    // Pass through variables.
    tc_position[gl_InvocationID] = v_position[gl_InvocationID].xyz;
    tc_instanceColor[gl_InvocationID] = v_instanceColor[gl_InvocationID];

    tc_particlePosition[gl_InvocationID] = v_particlePosition[gl_InvocationID];
    tc_particleScale[gl_InvocationID] = v_particleScale[gl_InvocationID];

    // Setting up tessellation levels.
    gl_TessLevelOuter[0] = 4;
    gl_TessLevelOuter[1] = 4;
    gl_TessLevelOuter[2] = 4;
    gl_TessLevelInner[0] = 4;
}

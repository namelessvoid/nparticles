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

layout(triangles) in;
in vec3 tc_position[];

in vec3 tc_particlePosition[];
in float tc_particleScale[];
flat in vec4 tc_instanceColor[];

out vec3 te_position;
out vec3 te_patchDistance;
flat out vec4 te_instanceColor;

uniform mat4 np_viewProjectionMatrix;

/**
 * Perform transformation of new vertex.
 */
void main()
{
    // Calculate new vertex positions.
    vec3 p0 = gl_TessCoord.x * tc_position[0];
    vec3 p1 = gl_TessCoord.y * tc_position[1];
    vec3 p2 = gl_TessCoord.z * tc_position[2];

    // Pass through planet color.
    te_instanceColor = tc_instanceColor[0];

    // Transform new vertex to its destination in the scene.
    te_patchDistance = gl_TessCoord;
    te_position = normalize(p0 + p1 + p2);
    gl_Position = np_viewProjectionMatrix * vec4(te_position * tc_particleScale[0] + tc_particlePosition[0] * 1.0e-8, 1);
}

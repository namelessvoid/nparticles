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

#include </np/gravity-utils.glsl>

#include </solarsystem/buffers.glsl>

#define timestep 900

/**
 * Update the planets using the improved euler integration.
 */
layout(local_size_x = planetCount) in;
void main()
{
    dvec3 position = positions[gl_LocalInvocationIndex].xyz;
    dvec3 oldVelocity = velocities[gl_LocalInvocationIndex].xyz;

    dvec3 acceleration = dvec3(0, 0, 0);

    for(int i = 0; i < planetCount; ++i)
    {
        acceleration += npCalcAcceleration(position, positions[i].xyz, positions[i].w, 1);
    }

    dvec3 newVelocity = oldVelocity + NP_GRAVITATIONAL_CONSTANT * acceleration * timestep;
    position += (newVelocity + oldVelocity) * 0.5 * timestep;

    // Fix sun position
    position = position * step(1, gl_LocalInvocationIndex);

    positions[gl_LocalInvocationIndex].xyz = position;
    velocities[gl_LocalInvocationIndex].xyz = newVelocity;
}

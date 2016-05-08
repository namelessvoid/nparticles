/*
 * Copyright (C) 2015 Simon Kerler
 *
 * This file is part of the "Nameless Particle Engine".
 * For conditions of distribution and use, see the copyright notice you should
 * have recievied with this software. I not, see:
 * http://opensource.org/licenses/Zlib
 */

#ifndef SOLARSYSTEM_BUFFERS_HPP
#define SOLARSYSTEM_BUFFERS_HPP

#define planetCount 10

/**
 * Buffer to store positions (xyz) and masses (w).
 */
layout(binding = 0) buffer Positions
{
    dvec4 positions[planetCount];
};

/**
 * Buffer to store velocities (xyz). w is unused.
 */
layout(binding = 1) buffer Velocities
{
    dvec4 velocities[planetCount];
};

/**
 * Uniform buffer that stores colors of the planets.
 */
layout(std140) uniform Colors
{
    vec4 colors[planetCount];
};

#endif // SOLARSYSTEM_BUFFERS_HPP

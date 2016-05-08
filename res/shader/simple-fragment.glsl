/*
 * Copyright (C) 2015 Simon Kerler
 *
 * This file is part of the "Nameless Particle Engine".
 * For conditions of distribution and use, see the copyright notice you should
 * have recievied with this software. I not, see:
 * http://opensource.org/licenses/Zlib
 */

#version 430

/**
 * The color of the fragment.
 */
out vec4 fColor;

/**
 * Very simple fragment shader, painting fragments red.
 *
 * TODO: This should be removed since it is provided by the
 *       MaterialManager's default material!
 */
void main(void)
{
    fColor = vec4(1, 0, 0, 1);
}

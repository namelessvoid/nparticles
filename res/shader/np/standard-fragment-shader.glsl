/*
 * Copyright (C) 2015 Simon Kerler
 *
 * This file is part of the "Nameless Particle Engine".
 * For conditions of distribution and use, see the copyright notice you should
 * have recievied with this software. I not, see:
 * http://opensource.org/licenses/Zlib
 */

#version 430

out vec4 f_np_color;

/**
 * Standard fragment shader painting all fragments yellow.
 */
void main()
{
    f_np_color = vec4(1.0, 1.0, 0.0, 1.0);
}

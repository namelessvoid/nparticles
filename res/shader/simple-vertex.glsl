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
 * Input variable storing the positin of a vertex.
 *
 * TODO: This is deprecated and should be accomplished using
 *       the engine built-in shader library!
 */
in vec3 np_in_position;

/**
 * Very simple pass-through vertex shader.
 *
 * TODO: Remove since this is provided by the MaterialManager's
 *       default material!
 */
void main()
{
    gl_Position = vec4(np_in_position, 1.0);
}

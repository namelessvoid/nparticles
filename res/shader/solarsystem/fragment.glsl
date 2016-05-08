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

flat in vec4 te_instanceColor;
out vec4 f_color;

/**
 * Simple fragment shader coloring the whole planet in a uniform color.
 */
void main()
{
    f_color = te_instanceColor;
}

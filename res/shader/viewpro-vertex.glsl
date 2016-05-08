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
 * Uniform variable storing the view projection matrix
 * of a camera.
 *
 * TODO: This is deprecated and should be accomplished using
 *       the engine built-in shader library!
 */
uniform mat4 np_viewProjectionMatrix;

/**
 * Buffer storing positins of particles.
 */
buffer b_position
{
    vec4[] position;
};

/**
 * A simple vertex shader performing vertex translation
 * and view projection.
 *
 * The former uses data of the b_positions buffer whereas
 * the latter uses the engine built-in uniform variable
 * np_viewProjectionMatrix.
 */
void main()
{
    vec3 pos = np_in_position + position[gl_InstanceID].xyz;

    gl_Position = np_viewProjectionMatrix * vec4(pos, 1.0);
}

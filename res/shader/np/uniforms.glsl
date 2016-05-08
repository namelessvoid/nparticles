/*
 * Copyright (C) 2015 Simon Kerler
 *
 * This file is part of the "Nameless Particle Engine".
 * For conditions of distribution and use, see the copyright notice you should
 * have recievied with this software. I not, see:
 * http://opensource.org/licenses/Zlib
 */

#ifndef NP_UNIFORMS_GLSL
#define NP_UNIFORMS_GLSL

/**
 * The view projection matrix of the camera.
 *
 * This uniform stores the most recent camera view projection matrix which can be used to
 * perform view projection inside the vertex shader.
 */
uniform mat4 np_viewProjectionMatrix;

uniform mat3 np_normalMatrix;

#endif // NP_UNIFORMS_GLSL

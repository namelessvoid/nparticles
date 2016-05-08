/*
 * Copyright (C) 2015 Simon Kerler
 *
 * This file is part of the "Nameless Particle Engine".
 * For conditions of distribution and use, see the copyright notice you should
 * have recievied with this software. I not, see:
 * http://opensource.org/licenses/Zlib
 */

#ifndef NP_GRAVITY_UTILS_GLSL
#define NP_GRAVITY_UTILS_GLSL

// ---------
// Constants
// ---------

/**
 * The gravitational constant.
 *
 * The gravitational constant as specified by CODATA in 2010.
 */
#define NP_GRAVITATIONAL_CONSTANT 6.67384e-11

/**
 * Calculate the acceleration caused by an attractor.
 *
 * This method calculates the acceleration an attractor impacts on a body.
 *
 * @note To provide greater flexibility and optimisation possibilities, this method does not take
 *       the gravitational constant (np_gravitational_constant) into account!
 *
 * @param particlePosition The position of the attracted body.
 * @param attractorPosition The position of the attractor.
 * @param attractorMass The mass of the attractor.
 * @param softeningFactor A softening factor used to prevent the acceleration from becoming infinite if
 *                        the distance between attracted and attractor is virtually zero.
 */
vec3 npCalcAcceleration(in vec3 attractedPosition, in vec3 attractorPosition, in float attractorMass, in float softeningFactor)
{
    // Calculate acceleration
    vec3 r = attractorPosition - attractedPosition;
    float distanceSquare = dot(r, r) + softeningFactor * softeningFactor;
    float divisor = sqrt(pow(distanceSquare, 3.0));
    return r * (attractorMass / divisor);
}

/**
 * Calculate the acceleration caused by an attractor.
 *
 * This method calculates the acceleration an attractor impacts on a body.
 *
 * @note To provide greater flexibility and optimisation possibilities, this method does not take
 *       the gravitational constant (np_gravitational_constant) into account!
 *
 * @param particlePosition The position of the attracted body.
 * @param attractorPosition The position of the attractor.
 * @param attractorMass The mass of the attractor.
 * @param softeningFactor A softening factor used to prevent the acceleration from becoming infinite if
 *                        the distance between attracted and attractor is virtually zero.
 */
dvec3 npCalcAcceleration(in dvec3 attractedPosition, in dvec3 attractorPosition, in double attractorMass, in float softeningFactor)
{
    // Calculate acceleration
    dvec3 r = attractorPosition - attractedPosition;
    double distanceSquare = dot(r, r) + softeningFactor * softeningFactor;
    double divisor = sqrt(distanceSquare * distanceSquare * distanceSquare);
    return r * (attractorMass / divisor);
}

#endif // NP_GRAVITY_UTILS_GLSL

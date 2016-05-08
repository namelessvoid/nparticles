/*
 * Copyright (C) 2015 Simon Kerler
 *
 * This file is part of the "Nameless Particle Engine".
 * For conditions of distribution and use, see the copyright notice you should
 * have recievied with this software. I not, see:
 * http://opensource.org/licenses/Zlib
 */

#ifndef NP_GLOBALINVOCATIONINDEX_HPP
#define NP_GLOBALINVOCATIONINDEX_HPP

/**
 * Calculate the global invocation index of a shader.
 *
 * The global invocation index is equivalent to gl_LocalInvocationIndex but also
 * considers the work grozp in which the work item resides.
 *
 * @note This has to be included *after* the local group size was specified, so
 * gl_WorkGroupSize is available!
 */
uint npGetGlobalInvocationIndex()
{
    return gl_GlobalInvocationID.x
         + gl_GlobalInvocationID.y * gl_WorkGroupSize.x
         + gl_GlobalInvocationID.z * gl_WorkGroupSize.x * gl_WorkGroupSize.y;
}

#endif

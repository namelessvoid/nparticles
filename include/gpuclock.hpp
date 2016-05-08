/*
 * Copyright (C) 2015 Simon Kerler
 *
 * This file is part of the "Nameless Particle Engine".
 * For conditions of distribution and use, see the copyright notice you should
 * have recievied with this software. I not, see:
 * http://opensource.org/licenses/Zlib
 */

#ifndef NP_GPUCLOCK_HPP
#define NP_GPUCLOCK_HPP

#include <GL/glew.h>

#include "clock.hpp"

namespace nparticles
{

/**
 * The GPUClock class can be used to measure GPU time.
 *
 * By using OpenGL queries, this Clock can measure the
 * time spent by the GPU.
 *
 * @note Limited by OpenGL, there can only be one active GPUClock! Calling start() for multiple
 * GPUClocks may result in OpenGL errors.
 */
class GPUClock : public Clock
{
public:
    /**
     * The GPUClock constructor.
     */
    GPUClock();

    /**
     * The GPUClock destructor.
     */
    ~GPUClock();

    /**
     * @copydoc Clock::start()
     *
     * @note Do not call start() for more than one clock since this may lead to OpenGL errors.
     */
    void start();

    /**
     * @copydoc Clock::stop()
     *
     * @note Make sure that all GPUClock%s are stopped before calling start() for any of them.
     */
    void stop();

    /**
     * @copydoc Clock::timeAvailable()
     */
    bool timeAvailable();

    /**
     * @copydoc Clock::getElapsedTime()
     */
    double getElapsedTime();

private:
    /**
     * The query object used to issue an OpenGL query.
     */
    GLuint mQuery;

    /**
     * Stores if the clock was started.
     */
    bool mStarted;

    /**
     * Stores if the clock was stopped.
     */
    bool mStopped;

};

} // namespace nparticles

#endif // NP_GPUCLOCK_HPP

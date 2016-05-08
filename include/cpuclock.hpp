/*
 * Copyright (C) 2015 Simon Kerler
 *
 * This file is part of the "Nameless Particle Engine".
 * For conditions of distribution and use, see the copyright notice you should
 * have recievied with this software. I not, see:
 * http://opensource.org/licenses/Zlib
 */

#ifndef NP_CPUCLOCK_HPP
#define NP_CPUCLOCK_HPP

#include "clock.hpp"

#include "GLFW/glfw3.h"

namespace nparticles
{

/**
 * The CPUClock class can be used to measure CPU time.
 *
 * The glfwGetTime() function is used to determine the CPU clock. This method
 * uses the moste accurate time measure methode available for the operating system.
 */
class CPUClock : public Clock
{
public:
    /**
     * The CPUClock constructor.
     */
    CPUClock();

    /**
     * The CPUClock destructor.
     */
    ~CPUClock();

    /**
     * @copydoc Clock::start()
     */
    void start();

    /**
     * @copydoc Clock::stop()
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
     * The time start() was called.
     */
    double mStartTime;

    /**
     * The time stop() was called.
     */
    double mStopTime;
};

} // namespace nparticles

#endif // NP_CPUCLOCK_HPP

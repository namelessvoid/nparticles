/*
 * Copyright (C) 2015 Simon Kerler
 *
 * This file is part of the "Nameless Particle Engine".
 * For conditions of distribution and use, see the copyright notice you should
 * have recievied with this software. I not, see:
 * http://opensource.org/licenses/Zlib
 */

#ifndef NP_CLOCK_HPP
#define NP_CLOCK_HPP

namespace nparticles
{

/**
 * The Clock class is used as base for all time measurement classes.
 *
 * It implements an interface for time measurement.
 */
class Clock
{
public:
    /**
     * Virtual destructor.
     */
    virtual ~Clock() {}

    /**
     * Start the time measurement.
     */
    virtual void start() = 0;

    /**
     * Stop the time measurement.
     */
    virtual void stop() = 0;

    /**
     * Check if the result of time measurement is available.
     *
     * This means, a call of getTIme() is safe to return the desired time interval.
     * @return True if the result is available.
     */
    virtual bool timeAvailable() = 0;

    /**
     * Get the elapsed time.
     *
     * Returns the time elapsed between calls of start() and stop().
     *
     * @return The elapsed time in seconds. -1 if timeAvailable() evaluates to false.
     */
    virtual double getElapsedTime() = 0;
};

} // namespace nparticles

#endif // NP_CLOCK_HPP

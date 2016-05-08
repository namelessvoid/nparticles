/*
 * Copyright (C) 2015 Simon Kerler
 *
 * This file is part of the "Nameless Particle Engine".
 * For conditions of distribution and use, see the copyright notice you should
 * have recievied with this software. I not, see:
 * http://opensource.org/licenses/Zlib
 */

#include "cpuclock.hpp"

namespace nparticles
{

CPUClock::CPUClock()
    : mStartTime(-1),
      mStopTime(-1)
{
}

CPUClock::~CPUClock()
{
}

void CPUClock::start()
{
    mStartTime = glfwGetTime();
    mStopTime = -1;
}

void CPUClock::stop()
{
    if(mStartTime != -1)
        mStopTime = glfwGetTime();
}

bool CPUClock::timeAvailable()
{
    return mStopTime != -1;
}

double CPUClock::getElapsedTime()
{
    if(!timeAvailable())
        return -1;
    return mStopTime - mStartTime;
}

} // namespace nparticles

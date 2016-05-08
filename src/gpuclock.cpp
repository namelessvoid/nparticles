/*
 * Copyright (C) 2015 Simon Kerler
 *
 * This file is part of the "Nameless Particle Engine".
 * For conditions of distribution and use, see the copyright notice you should
 * have recievied with this software. I not, see:
 * http://opensource.org/licenses/Zlib
 */

#include "gpuclock.hpp"

namespace nparticles
{

GPUClock::GPUClock()
    : mQuery(0),
      mStarted(false),
      mStopped(false)
{
}

GPUClock::~GPUClock()
{
    glDeleteQueries(1, &mQuery);
}

void GPUClock::start()
{
    // Stop the time query to restart it.
    if(mStarted)
        glEndQuery(GL_TIME_ELAPSED);

    if(!mQuery)
        glGenQueries(1, &mQuery);

    glBeginQuery(GL_TIME_ELAPSED, mQuery);
    mStarted = true;
    mStopped = false;
}

void GPUClock::stop()
{
    if(mStarted && !mStopped)
    {
        glEndQuery(GL_TIME_ELAPSED);
        mStarted = false;
        mStopped = true;
    }
}

bool GPUClock::timeAvailable()
{
    if(!mStopped)
        return false;
    GLuint status;
    glGetQueryObjectuiv(mQuery, GL_QUERY_RESULT_AVAILABLE, &status);
    return status == GL_TRUE;
}

double GPUClock::getElapsedTime()
{
    if(!mStopped)
        return -1;
    GLuint64 result;
    glGetQueryObjectui64v(mQuery, GL_QUERY_RESULT, &result);

    return ((double)result) / 1000000000;
}

} // namespace nparticles

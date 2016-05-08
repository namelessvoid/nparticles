/*
 * Copyright (C) 2015 Simon Kerler
 *
 * This file is part of the "Nameless Particle Engine".
 * For conditions of distribution and use, see the copyright notice you should
 * have recievied with this software. I not, see:
 * http://opensource.org/licenses/Zlib
 */

#include "buffer.hpp"

namespace nparticles
{

BufferBase::BufferBase(int itemCount)
    : mBufferHandle(0),
      mItemCount(itemCount),
      mGlTypeInfo(GL_INVALID_ENUM, -1),
      mCurrentlyBound(false),
      mCurrentTarget(0),
      mCurrentIndex(0)
{
    glGenBuffers(1, &mBufferHandle);
}

BufferBase::~BufferBase()
{
    glDeleteBuffers(1, &mBufferHandle);
}

void BufferBase::bind(GLenum target)
{
    // First unbind if necessary.
    unbind();

    glBindBuffer(target, mBufferHandle);

    mCurrentTarget = target;
    mCurrentIndex = 0;
    mCurrentlyBound = true;
}

void BufferBase::bindBase(GLenum target, GLuint index)
{
    // First unbind if necessary;
    unbind();

    glBindBufferBase(target, index, mBufferHandle);

    mCurrentTarget = target;
    mCurrentIndex = index;
    mCurrentlyBound = true;
}

void BufferBase::unbind()
{
    if(!mCurrentlyBound)
        return;

    if(mCurrentIndex > 0)
        glBindBufferBase(mCurrentTarget, mCurrentIndex, 0);
    else
        glBindBuffer(mCurrentTarget, 0);
}

}

/*
 * Copyright (C) 2015 Simon Kerler
 *
 * This file is part of the "Nameless Particle Engine".
 * For conditions of distribution and use, see the copyright notice you should
 * have recievied with this software. I not, see:
 * http://opensource.org/licenses/Zlib
 */

#include "atomiccounterbuffer.hpp"

namespace nparticles
{

AtomicCounterBuffer::AtomicCounterBuffer(int itemCount)
    : Buffer<GLuint>(itemCount, GL_UNSIGNED_INT, 1, GL_DYNAMIC_COPY, GL_ATOMIC_COUNTER_BUFFER)
{
}

} // namespace nparticles

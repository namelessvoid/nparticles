/*
 * Copyright (C) 2015 Simon Kerler
 *
 * This file is part of the "Nameless Particle Engine".
 * For conditions of distribution and use, see the copyright notice you should
 * have recievied with this software. I not, see:
 * http://opensource.org/licenses/Zlib
 */

#ifndef NP_ATOMICCOUNTERBUFFER_HPP
#define NP_ATOMICCOUNTERBUFFER_HPP

#include "buffer.hpp"

namespace nparticles
{

/**
 * The AtomicCounterBuffer class is used to abstract atomic counter buffers.
 *
 * This is a convenience class because atomic counter must be of tyle GLuint. It also
 * supports a binding index which is mandatory for the layout qualifier of atomic counters
 * in shader programs.
 *
 * This simplifies the binding process of atomic counter buffers, since the binding index is not
 * queried at run time using the ShaderProgarm API.
 */
class AtomicCounterBuffer : public Buffer<GLuint>
{
public:
    /**
     * The AtomicCounterBuffer constructor.
     *
     * This initialises a new atomic counter buffer. As for all buffers, the @p itemCount has to be specified
     * because buffers are designed to be immutable. The binding index is used to bind the atomic counter buffer
     * to the binding index used in the shader program.
     *
     * @param itemCount The number of atomic counters stored in the buffer.
     */
    AtomicCounterBuffer(int itemCount);
};


} // namespace nparticles

#endif // NP_ATOMICCOUNTERBUFFER_HPP

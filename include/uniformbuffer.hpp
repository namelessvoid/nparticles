/*
 * Copyright (C) 2015 Simon Kerler
 *
 * This file is part of the "Nameless Particle Engine".
 * For conditions of distribution and use, see the copyright notice you should
 * have recievied with this software. I not, see:
 * http://opensource.org/licenses/Zlib
 */

#ifndef NP_UNIFORMBUFFER_HPP
#define NP_UNIFORMBUFFER_HPP

#include "buffer.hpp"

namespace nparticles
{

/**
 * The UniformBuffer class represents uniform buffers.
 *
 * This class is used to create a uniform buffer.
 */
template<typename T>
class UniformBuffer : public Buffer<T>
{
public:
    /**
     * UniformBuffer constructor.
     *
     * This takes a mandatory @p itemCount parameter and information about the stored type (see Buffer).
     * A new uniform buffer is created.
     *
     * @see Buffer
     *
     * @param itemCount The number of items in the buffer.
     * @param type The OpenGL type of data stored int he buffer. See glutils::GlTypeInfo.
     * @param glBaseSize The base size of the gl type. See glutils::GlTypeInfo.
     */
    UniformBuffer(int itemCount, GLenum type = GL_INVALID_VALUE, int glBaseSize = -1)
        : Buffer<T>(itemCount, type, glBaseSize, GL_DYNAMIC_DRAW, GL_UNIFORM_BUFFER)
    {
    }
};

} // namespace nparticles

#endif // NP_UNIFORMBUFFER_HPP

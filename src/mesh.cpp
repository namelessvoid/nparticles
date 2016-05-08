/*
 * Copyright (C) 2015 Simon Kerler
 *
 * This file is part of the "Nameless Particle Engine".
 * For conditions of distribution and use, see the copyright notice you should
 * have recievied with this software. I not, see:
 * http://opensource.org/licenses/Zlib
 */

#include "mesh.hpp"

namespace nparticles
{

Mesh::Mesh(size_t vertexCount, glm::vec3* vertices, size_t indexCount, GLuint* indices)
    : mVertexBuffer(vertexCount, GL_INVALID_VALUE, -1, GL_STATIC_DRAW, GL_ARRAY_BUFFER),
      mIndexBuffer(indexCount, GL_INVALID_VALUE, -1, GL_STATIC_DRAW, GL_ELEMENT_ARRAY_BUFFER)
{
    glGenVertexArrays(1, &mVao);
    mVertexBuffer.setData(vertices);
    mIndexBuffer.setData(indices);
}

Mesh::~Mesh()
{
    glDeleteVertexArrays(1, &mVao);
}

void Mesh::bind() const
{
    glBindVertexArray(mVao);
}

void Mesh::unbind() const
{
    // Maybe we have to add a isBound check... not sure about that.
    glBindVertexArray(0);
}

} // namespace nparticles

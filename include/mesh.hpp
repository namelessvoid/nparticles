/*
 * Copyright (C) 2015 Simon Kerler
 *
 * This file is part of the "Nameless Particle Engine".
 * For conditions of distribution and use, see the copyright notice you should
 * have recievied with this software. I not, see:
 * http://opensource.org/licenses/Zlib
 */

#ifndef NP_MESH_HPP
#define NP_MESH_HPP

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "buffer.hpp"

namespace nparticles
{

class MeshManager;

template<typename T>
class ResourceManager;

/**
 * The Mesh class is used to store geometry data.
 *
 * A Mesh is a piece of geometry consisting of vertices. For rendering, a list of vertex indices
 * is defined, which define the order of the vertices.
 *
 * It can be used to store per vertex data like texture coordinates which are currently not supported.
 *
 * Meshes are managed by the MeshManager and can not be created manually.
 */
class Mesh
{
friend class MeshManager;
friend class ResourceManager<Mesh>;
public:
    /**
     * Binds a Mesh for rendering.
     *
     * This activates the Mesh so it can be rendered.
     */
    void bind() const;

    /**
     * Unbinds the Mesh after rendering.
     *
     * This disables the Mesh after rendering completed.
     */
    void unbind() const;

    /**
     * Get the vertx Buffer of the Mesh.
     *
     * This returns the veretx Buffer of the Mesh.
     *
     * @return Pointer to the vertex buffer.
     */
    Buffer<glm::vec3>* getVertexBuffer() { return &mVertexBuffer; }

    /**
     * Get the index Buffer of the Mesh.
     *
     * This returns the index buffer of the Mesh.B
     *
     * @return Pointer to the index Buffer of the Mesh.
     */
    Buffer<GLuint>* getIndexBuffer() { return &mIndexBuffer; }

private:
    /**
     * Private Mesh constructor.
     *
     * Since Meshes are manged by the MeshManager, this method must not be called manually.
     *
     * A new Mesh is created with given vertices and indices.
     *
     * @param vertexCount The number of vertices.
     * @param vertices Array of size @p vertexCount which contains the vertices.
     * @param indexCount The number of indices.
     * @param indices Array of size @p indexCount which contains the indices.
     */
    Mesh(size_t vertexCount, glm::vec3* vertices, size_t indexCount, GLuint* indices);

    /**
     * The Mesh destructor.
     */
    ~Mesh();

    /**
     * This is the vertex array object (VAO) used for binding of vertex attributes by OpenGL.
     */
    GLuint mVao;

    /**
     * The vertex Buffer.
     *
     * This Buffer contains the vertices of the geometry.
     */
    Buffer<glm::vec3> mVertexBuffer;

    /**
     * The index Buffer.
     *
     * This Buffer contains the indices of the geometry.
     */
    Buffer<GLuint> mIndexBuffer;

    // Hide copy constructor and assignment operator
    Mesh(const Mesh&) = delete;
    void operator=(const Mesh&) = delete;
};

} // namesapce nparticles

#endif // NP_MESH_HPP

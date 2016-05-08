/*
 * Copyright (C) 2015 Simon Kerler
 *
 * This file is part of the "Nameless Particle Engine".
 * For conditions of distribution and use, see the copyright notice you should
 * have recievied with this software. I not, see:
 * http://opensource.org/licenses/Zlib
 */

#include "meshmanager.hpp"

#include "logger.hpp"

namespace nparticles
{

MeshManager::MeshManager()
{
}

MeshManager::~MeshManager()
{
}

const Mesh* MeshManager::createMesh(std::string id, size_t vertexCount, glm::vec3* vertices, size_t indexCount, GLuint* indices)
{
    if(exists(id))
    {
        Logger::getInstance()->logWarning("MeshManager: Mesh with id \"" + id + "\" already exists!");
        return nullptr;
    }

    Mesh* mesh = new Mesh(vertexCount, vertices, indexCount, indices);
    mResourceMap[id] = mesh;
    return mesh;
}

const Mesh* MeshManager::createIcosahedron(std::string id)
{
    if(exists(id))
    {
        Logger::getInstance()->logWarning("MeshManager:: Mesh with id \"" + id + "\" already exists!");
        return nullptr;
    }

    // Code from OpenGL Programming Guide, Version 3
    const float x = 0.525731112119133606;
    const float z = 0.850650808352039932;

    glm::vec3 vertices[12] = {
        {-x, 0.0, z}, {x, 0.0, z}, {-x, 0.0, -z}, {x, 0.0, -z},
        {0.0, z, x}, {0.0, z, -x}, {0.0, -z, x}, {0.0, -z, -x},
        {z, x, 0.0}, {-z, x, 0.0}, {z, -x, 0.0}, {-z, -x, 0.0}
    };


    GLuint indices[20][3] = {
        {1, 4, 0}, {4, 9, 0}, {4, 5, 9}, {8, 5, 4},	{1, 8, 4},
        {1, 10, 8}, {10, 3, 8}, {8, 3, 5}, {3, 2, 5}, {3, 7, 2},
        {3, 10, 7}, {10, 6, 7}, {6, 11, 7}, {6, 0, 11},	{6, 1, 0},
        {10, 1, 6}, {11, 0, 9}, {2, 11, 9}, {5, 2, 9}, {11, 2, 7},
    };

    // Create the mesh
    const Mesh* mesh = createMesh(id, 12, vertices, 60, &indices[0][0]);
    return  mesh;
}

void MeshManager::createDefaultResource()
{
    glm::vec3 vertex(0, 0, 0);
    GLuint index = 0;

    mDefaultResource = new Mesh(1, &vertex, 1, &index);
}

} // namespace nparticles

/*
 * Copyright (C) 2015 Simon Kerler
 *
 * This file is part of the "Nameless Particle Engine".
 * For conditions of distribution and use, see the copyright notice you should
 * have recievied with this software. I not, see:
 * http://opensource.org/licenses/Zlib
 */

#ifndef NP_MESHMANAGER_HPP
#define NP_MESHMANAGER_HPP

#include <string>

#include "singleton.hpp"
#include "resourcemanager.hpp"
#include "mesh.hpp"

namespace nparticles
{

/**
 * A ResourceManager which manages Mesh objects.
 *
 * This ResoruceManger manages Mesh objects.
 */
class MeshManager : public Singleton<MeshManager>, public ResourceManager<Mesh>
{
friend class Singleton<MeshManager>;

public:
    /**
     * Create a new managed Mesh.
     *
     * Create a new Mesh and add it to the managed Meshes. For Details about Mesh creation see Mesh.
     *
     * @param id The ID of the new Mesh.
     * @param vertexCount The number of vertices for the new Mesh.
     * @param vertices Array of vertices for the Mesh.
     * @param indexCount The number of indices for the new Mesh.
     * @param indices Array of indices for the Mesh.
     *
     * @return Pointer to the newly created Mesh or null pointer if a Mesh with @p id already exists.
     */
    const Mesh* createMesh(std::string id, size_t vertexCount, glm::vec3* vertices, size_t indexCount, GLuint* indices);

    /**
     * Create a new icosahedron Mesh.
     *
     * Create a new Mesh that is an icosahedron. Add it to the managed meshes and return a pointer
     * to it.
     *
     * @param id The ID of the new Mesh.
     * @return Pointer to the newly created icosahedron Mesh or null pointer if a Mesh with @p id already exists.
     */
    const Mesh* createIcosahedron(std::string id);

    /**
     * Get a Mesh by its ID.
     *
     * This is a convenience wrapper method of ResourceManager::getResource().
     *
     * @param id The string ID of the Mesh.
     *
     * @return Pointer to the requested Mesh or null pointer if no Mesh with ID @id exists.
     */
    inline const Mesh* getMesh(std::string id) { return getResource(id); }

    /**
     * Get the default Mesh.
     *
     * This is a convenience wrapper method of ResourceManager::getDefaultResource().
     *
     * The default Mesh has only one vertex (0, 0, 0).
     *
     * @return
     */
    inline const Mesh* getDefaultMesh() { return getDefaultResource(); }

private:
    /**
     * Private MeshManager constructor.
     */
    MeshManager();

    /**
     * Private MeshManager destructor.
     */
    ~MeshManager();

    /**
     * Create the default Mesh.
     *
     * This method is only called once, if ResourceManager::getDefaultResource() is called the first time.
     */
    void createDefaultResource();

    // Hide copy constructor and assignment operator
    MeshManager(const MeshManager&) = delete;
    void operator=(const MeshManager&) = delete;

};

}

#endif // NP_MESHMANAGER_HPP

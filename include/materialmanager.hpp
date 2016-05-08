/*
 * Copyright (C) 2015 Simon Kerler
 *
 * This file is part of the "Nameless Particle Engine".
 * For conditions of distribution and use, see the copyright notice you should
 * have recievied with this software. I not, see:
 * http://opensource.org/licenses/Zlib
 */

#ifndef NP_MATERIALMANAGER_HPP
#define NP_MATERIALMANAGER_HPP

#include "singleton.hpp"
#include "resourcemanager.hpp"
#include "material.hpp"

namespace nparticles
{

class RenderProgram;

/**
 * A ResourceManager for Material objects.
 *
 * This ResourceManager manages Material objects.
 */
class MaterialManager : public ResourceManager<Material>, public Singleton<MaterialManager>
{
friend class Singleton<MaterialManager>;

public:
    /**
     * Create a new managed Material.
     *
     * Create a new Material and add it to the managed Materials. For details about Material creation see Material.
     *
     * @param id The string ID of the new Material.
     * @param renderProgram The RenderProgram used by the Material.
     * @param renderType One of the nparticles::render_types.
     *
     * @return Pointer to the newly created Material or null pointer if a Mesh with @p id already exists.
     */
    const Material* createMaterial(const std::string& id, RenderProgram* renderProgram, render_types renderType = NP_RT_TRIANGLES);

    /**
     * Create a new managed Material.
     *
     * Create a new Material and add it to the managed Materials. For details about Material creation see Material.
     *
     * @param id The string ID of the new Material.
     * @param renderProgramId The string ID of a RenderProgram to be used by the Material.
     * @param renderType One of the nparticles::render_types.
     * @return
     */
    const Material* createMaterial(const std::string& id, const std::string& renderProgramId, render_types renderType = NP_RT_TRIANGLES);

    /**
     * Get a Material by its ID.
     *
     * This is a convenience wrapper method for ResourceManager::getResource().
     *
     * @param id The string ID of the requested Material.
     *
     * @return Pointer to the requested Material or null pointer, if no Material with ID @p id exists.
     */
    inline const Material* getMaterial(const std::string& id) { return getResource(id); }

    /**
     * Get the default material.
     *
     * This is a convenience wrapper method for ResourceManager::getDefaultResource().
     *
     * The default Material uses the default RenderProgram retrievable with GPUProgramService::getDefaultRenderProgram().
     *
     * @return Pointer to the default Material.
     */
    inline const Material* getDefaultMaterial() { return getDefaultResource(); }


private:
    /**
     * Private MaterialManager constructor.
     */
    MaterialManager();

    /**
     * Private MaterialManager destructor.
     */
    ~MaterialManager();

    /**
     * Create the default Material.
     *
     * This method is only called once, if ResourceManager::getDefaultResource() is called the first time.
     */
    void createDefaultResource();

    // Hide copy constructor and assignment operator
    MaterialManager(const MaterialManager&) = delete;
    void operator=(const MaterialManager&) = delete;
};

} // namespace nparticles

#endif // NP_MATERIAL_MANAGER

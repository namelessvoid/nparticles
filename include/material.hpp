/*
 * Copyright (C) 2015 Simon Kerler
 *
 * This file is part of the "Nameless Particle Engine".
 * For conditions of distribution and use, see the copyright notice you should
 * have recievied with this software. I not, see:
 * http://opensource.org/licenses/Zlib
 */

#ifndef NP_MATERIAL_HPP
#define NP_MATERIAL_HPP

#include "renderprogram.hpp"

namespace nparticles
{

template<typename T>
class ResourceManager;

/**
 * @brief The render_types enum defines supported render types.
 */
enum render_types
{
    /**
     * Render geometry as triangles.
     */
    NP_RT_TRIANGLES,

    /**
     * Render geometry as points.
     */
    NP_RT_POINTS
};

/**
 * The Material class used to store rendering information.
 *
 * A Material stores everything that is required to render a geometry (i.e. Mesh.
 */
class Material
{
friend class MaterialManager;
friend class ResourceManager<Material>;

public:
    /**
     * Get the RenderProgram attached to the Material.
     *
     * Get the RenderProgram that is used to render with this Material.
     *
     * @return Pointer to the RenderProgram.
     */
    inline RenderProgram* getRenderProgram() const { return &mRenderProgram; }

    /**
     * Get the render type.
     *
     * Get the render type which is used to render with this Material.
     *
     * @see nparticles::render_types
     *
     * @return One of the available render types.
     */
    render_types getRenderType() const { return mRenderType; }

    /**
     * Set the render type.
     *
     * Set the render type which is used to render with this Material.
     *
     * @see nparticles::render_types
     *
     * @param renderType One of the available render types.
     */
    void setRenderType(render_types renderType) { mRenderType = renderType; }

private:
    /**
     * Private Material constructor.
     *
     * Since Materials are managed by the MaterialManager, this method must not be called manually.
     *
     * A new Material is created with given RenderProgram and render type @p renderType.
     *
     * @param renderProgram The RenderProgram used to render with this Material.
     * @param renderType One of the available nparticles::render_types.
     */
    Material(RenderProgram& renderProgram, render_types renderType = NP_RT_TRIANGLES);

    /**
     * Private Material destructor.
     */
    ~Material();

    /**
     * The RenderProgram used to render with this Material.
     */
    RenderProgram& mRenderProgram;

    /**
     * The render type to render with this Material.
     *
     * @see nparticles::render_types
     */
    render_types mRenderType;

    // Hide copy constructor and assignment operator
    Material(const Material&) = delete;
    void operator=(const Material&) = delete;
};

} // namespace nparticles

#endif // NP_MATERIAL_HPP

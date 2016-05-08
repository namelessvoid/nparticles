/*
 * Copyright (C) 2015 Simon Kerler
 *
 * This file is part of the "Nameless Particle Engine".
 * For conditions of distribution and use, see the copyright notice you should
 * have recievied with this software. I not, see:
 * http://opensource.org/licenses/Zlib
 */

#ifndef NP_RENDERPROGRAM_HPP
#define NP_RENDERPROGRAM_HPP

#include "shaderprogram.hpp"

namespace nparticles
{

/**
 * The RenderProgram class encapsulates render shader programs.
 *
 * It requires at least a vertex and a fragment shader, but tesselation control,
 * tesselation evaluation and geometry shaders can be also used.
 *
 * They are managed by the GPUProgramService and must not be instantiated manually.
 */
class RenderProgram : public ShaderProgram
{
friend class GPUProgramService;

public:
    /**
     * Get the location of a vertex attribute.
     *
     * Queries the location index of a vertex attribute of the RenderProgram.
     *
     * If you have declared a vertex attribute but cannot retrieve its location, it maybe fell a victim to
     * the drivers compiler optimisations.
     *
     * @param attributeName String containing the name of the vertex attribute that is queried.
     *
     * @return The attribute location of the vertex attribute within the shader program or -1
     *         if no vertex attribute with given name exists.
     */
    GLint getVertexAttributeLocation(const std::string& attributeName) const;

    /**
     * Get if this RenderProgram uses tessellation.
     *
     * This method returns whether this RenderProgram contains tessellation shaders (tessellation control / tessellation evaluation shaders)
     * or not.
     *
     * @return True if tessellation is used false otherwise.
     */
    inline bool usesTessellation() { return mUseTesselation; }

    /**
     * Set the vertices per tessellation patch.
     *
     * Set the size of a tessellation patch. This value defaults to 3.
     *
     * @note If @p patchSize is not a valied size (i.e. it is less than one or greater than GL_MAX_PATCH_VERTICES), it defaults
     * to zero and GL_MAX_PATCH_VERTICES respectively.
     *
     * @param patchSize The patch size used for tessellation.
     */
    void setTessellationPatchSize(int patchSize);

    /**
     * Get the tessellation patch size.
     *
     * Get the tessellation patch size previously set by setTessellationPatchSize(). Defaults to 3 if not changed by setTessellationPatchSize().
     *
     * @return The number of vertices in a tessellation patch.
     */
    inline GLint getTessellationPatchSize() { return mTessellationPatchSize; }

private:
    /**
     * Contsructor for RenderProgram.
     *
     * All RenderPrograms are managed by a GPUProgramService class which is used to create
     * RenderPrograms so you should not call this constructor manually.
     *
     * @param vertSrc String containing the source code for vertex shader (must not be empty!).
     * @param fragSrc String containing the source code for fragment shader (must not be empty!).
     * @param tscSrc String containing the source code for tesselation control shader (allowed to be empty).
     * @param tesSrc String containing the source code for tesselation evaluation shader (allowed to be empty).
     * @param geoSrc String containing the source code for geometry shader (allowed to be empty).
     */
    RenderProgram(std::string vertSrc, std::string fragSrc, std::string tscSrc, std::string tesSrc, std::string geoSrc);

    /**
     * Whether this RenderProgram uses tessellation or not.
     *
     * This is set in the constructor, if a tessellation control or tessellation evaluation shader is provided.
     */
    bool mUseTesselation;

    /**
     * The vertices of a tessellation patch.
     *
     * Defaults to 3. To change the size, use setTessellationPatchSize(). To get the size, use
     * getTessellationPatchSize().
     */
    GLint mTessellationPatchSize;

    // Hide copy constructor and assignment operator
    RenderProgram(const RenderProgram&) = delete;
    void operator=(const RenderProgram&) = delete;
};

} // namespace nparticles

#endif // NP_RENDERPROGRAM_HPP

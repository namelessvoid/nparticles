/*
 * Copyright (C) 2015 Simon Kerler
 *
 * This file is part of the "Nameless Particle Engine".
 * For conditions of distribution and use, see the copyright notice you should
 * have recievied with this software. I not, see:
 * http://opensource.org/licenses/Zlib
 */

#ifndef NP_COMPUTEPROGRAM_HPP
#define NP_COMPUTEPROGRAM_HPP

#include "shaderprogram.hpp"

namespace nparticles
{

/**
 * The ComputeProgram class encapsulates a compute shader program.
 *
 * Unless RenderProgram%s, a ComputeProgram only consists of one compute shader.
 *
 * They are created by the GPUProgramService and must not be instantiated manually.
 */
class ComputeProgram : public ShaderProgram
{
friend class GPUProgramService;

public:
    /**
     * Get the number of work items per work group.
     *
     * This is th produkt of the work group sizes x, y and z. The work group sizes
     * are queried directly from the shader when the ComputeProgram is built.
     *
     * @return The number of work items per work group.
     */
    inline uint getNumWorkItemsPerGroup() { return mNumWorkItemsPerGroup; }

    /**
     * @copydoc ShaderProgram::build()
     */
    bool build();

private:
    /**
     * ComputeProgram constructor.
     *
     * This creates the ComputeProgram with one compute shader attached.
     *
     * @param src The source of the compute shader.
     */
    ComputeProgram(std::string src);

    /**
     * The numbers of work items per work group.
     */
    uint mNumWorkItemsPerGroup;

    /**
     * The size of a work group as specified in the shader source.
     */
    int mWorkGroupSize[3];

    // Hide copy and assignment operators
    ComputeProgram(const ComputeProgram&) = delete;
    void operator=(const ComputeProgram&) = delete;
};

}

#endif // NP_COMPUTEPROGRAM_HPP

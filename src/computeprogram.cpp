/*
 * Copyright (C) 2015 Simon Kerler
 *
 * This file is part of the "Nameless Particle Engine".
 * For conditions of distribution and use, see the copyright notice you should
 * have recievied with this software. I not, see:
 * http://opensource.org/licenses/Zlib
 */

#include "computeprogram.hpp"

namespace nparticles
{

ComputeProgram::ComputeProgram(std::string src)
    : mNumWorkItemsPerGroup(0),
      mWorkGroupSize{0, 0, 0}
{
    addNewShader(GL_COMPUTE_SHADER, src);
}

bool ComputeProgram::build()
{
    if(!ShaderProgram::build())
        return false;

    // Get the work group size and number of work items per group
    glGetProgramiv(mShaderProgram, GL_COMPUTE_WORK_GROUP_SIZE, mWorkGroupSize);
    mNumWorkItemsPerGroup = mWorkGroupSize[0] * mWorkGroupSize[1] * mWorkGroupSize[2];

    return true;
}

} // namespace nparticles

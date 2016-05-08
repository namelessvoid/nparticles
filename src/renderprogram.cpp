/*
 * Copyright (C) 2015 Simon Kerler
 *
 * This file is part of the "Nameless Particle Engine".
 * For conditions of distribution and use, see the copyright notice you should
 * have recievied with this software. I not, see:
 * http://opensource.org/licenses/Zlib
 */

#include "renderprogram.hpp"

namespace nparticles
{

GLint RenderProgram::getVertexAttributeLocation(const std::string& attributeName) const
{
    return glGetAttribLocation(mShaderProgram, attributeName.c_str());
}

void RenderProgram::setTessellationPatchSize(int patchSize)
{
    patchSize = (patchSize > 0) ? patchSize : 1;

    GLint maxVertices = glutils::glGet(GL_MAX_PATCH_VERTICES);
    patchSize = (patchSize < maxVertices) ? patchSize : maxVertices;

    mTessellationPatchSize = patchSize;
}

RenderProgram::RenderProgram(std::string vertSrc, std::string fragSrc, std::string tscSrc, std::string tesSrc, std::string geoSrc)
    : mUseTesselation(false),
      mTessellationPatchSize(3)

{
    addNewShader(GL_VERTEX_SHADER, vertSrc);
    addNewShader(GL_FRAGMENT_SHADER, fragSrc);

    if(tscSrc.length() > 0)
    {
        mUseTesselation = true;
        addNewShader(GL_TESS_CONTROL_SHADER, tscSrc);
    }

    if(tesSrc.length() > 0)
    {
        mUseTesselation = true;
        addNewShader(GL_TESS_EVALUATION_SHADER, tesSrc);
    }

    if(geoSrc.length() > 0)
        addNewShader(GL_GEOMETRY_SHADER, geoSrc);
}

} // namespace nparticles

/*
 * Copyright (C) 2015 Simon Kerler
 *
 * This file is part of the "Nameless Particle Engine".
 * For conditions of distribution and use, see the copyright notice you should
 * have recievied with this software. I not, see:
 * http://opensource.org/licenses/Zlib
 */

#include "material.hpp"

namespace nparticles
{

Material::Material(RenderProgram& renderProgram, render_types renderType)
    : mRenderProgram(renderProgram),
      mRenderType(renderType)
{
}

Material::~Material()
{
}

} // namespace nparticles

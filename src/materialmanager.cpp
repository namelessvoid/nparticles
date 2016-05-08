/*
 * Copyright (C) 2015 Simon Kerler
 *
 * This file is part of the "Nameless Particle Engine".
 * For conditions of distribution and use, see the copyright notice you should
 * have recievied with this software. I not, see:
 * http://opensource.org/licenses/Zlib
 */

#include "materialmanager.hpp"

#include "logger.hpp"
#include "engine.hpp"

namespace nparticles
{

MaterialManager::MaterialManager()
{
}

MaterialManager::~MaterialManager()
{
}

const Material* MaterialManager::createMaterial(const std::string& id, RenderProgram* renderProgram, render_types renderType)
{
    if(exists(id))
    {
        Logger::getInstance()->logWarning("MaterialManager: Material with id \"" + id + "\" already exists!");
        return nullptr;
    }

    Material* material = new Material(*renderProgram, renderType);
    mResourceMap[id] = material;
    return material;
}

const Material* MaterialManager::createMaterial(const std::string& id, const std::string& renderProgramId, render_types renderType)
{
    RenderProgram* renderProgram = Engine::getInstance()->getGPUProgramService()->getRenderProgram(renderProgramId);

    if(!renderProgram)
    {
        Logger::getInstance()->logWarning("MaterialManager: RenderProgram with id \"" + renderProgramId + "\" does not exist.");
        return nullptr;
    }

    return createMaterial(id, renderProgram, renderType);
}

void MaterialManager::createDefaultResource()
{
    RenderProgram* renderProgram = Engine::getInstance()->getGPUProgramService()->getDefaultRenderProgram();
    mDefaultResource = new Material(*renderProgram, NP_RT_POINTS);
}

} // namespace nparticles

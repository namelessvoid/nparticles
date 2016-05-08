/*
 * Copyright (C) 2015 Simon Kerler
 *
 * This file is part of the "Nameless Particle Engine".
 * For conditions of distribution and use, see the copyright notice you should
 * have recievied with this software. I not, see:
 * http://opensource.org/licenses/Zlib
 */

/**
 * This sample renders a yellow icosahedron which is provided
 * as a standard mesh by the MeshManager.
 *
 * The mesh is rendered in plain yellow since no lighting is applied.
 **/

#include "engine.hpp"
#include "particlesystem.hpp"

using namespace nparticles;

int main(int argc, char* argv[])
{
    Engine* engine = Engine::getInstance();
    engine->init(1027, 768, false);
    engine->getCamera().move(glm::vec3(0, 0, -3));

    GPUProgramService* gpuProgramService = engine->getGPUProgramService();
    if(!gpuProgramService->addSourceDirectory("../res/shader/np", "/np"))
        return -1;

    if(!gpuProgramService->createRenderProgram("renderer", "/np/standard-vertex-shader.glsl", "/np/standard-fragment-shader.glsl"))
        return -1;

    const Material* material = engine->getMaterialManager()->createMaterial("material", "renderer");
    const Mesh* mesh = engine->getMeshManager()->createIcosahedron("icosahedron");

    engine->createParticleSystem(1, *mesh, *material);

    while(!engine->windowClosed())
    {
        engine->processEvents();
        engine->drawAllParticleSystems();
    }

    engine->terminate();
    return 0;
}

/*
 * Copyright (C) 2015 Simon Kerler
 *
 * This file is part of the "Nameless Particle Engine".
 * For conditions of distribution and use, see the copyright notice you should
 * have recievied with this software. I not, see:
 * http://opensource.org/licenses/Zlib
 */

/**
 * This sample demonstrates how particle system events
 * are used.
 *
 * Callbacks are registered to all 4 events which print
 * a message to stdout.
 *
 * Particles are not updated but rendered fixed at the origin of
 * the simulation.
 **/

#include <iostream>

#include "engine.hpp"
#include "particlesystem.hpp"

using namespace nparticles;

void preRenderCallback(ParticleSystem* pSys, RenderSystem* renderSystem)
{
    std::cout << "Pre render callback\n";
}

void postRenderCallback(ParticleSystem* pSys, RenderSystem* renderSystem)
{
    std::cout << "Post render callback\n";
}

void preUpdateCallback(ParticleSystem* psys, const ComputeSystem* computeSystem)
{
    std::cout << "Pre update callback\n";
}

void postUpdateCallback(ParticleSystem* psys, const ComputeSystem* computeSystem)
{
    std::cout << "Post update callback\n" << std::endl;
}

int main(int argc, char* argv[])
{
    Engine* engine = Engine::getInstance();
    engine->init(1024, 768, false);

    GPUProgramService* gpuProgramService = engine->getGPUProgramService();
    gpuProgramService->addSourceFile("../res/shader/dummy-compute.glsl", "/dummy-compute");
    ComputeProgram* computeProgram = gpuProgramService->createComputeProgram("computeProgram", "/dummy-compute");

    ParticleSystem* pSys = engine->createParticleSystem(1024, *engine->getMeshManager()->getDefaultMesh(), *engine->getMaterialManager()->getDefaultMaterial());
    Action* action = pSys->appendAction(*computeProgram);

    pSys->preRenderSignal.connect(preRenderCallback);
    pSys->postRenderSignal.connect(postRenderCallback);

    action->preUpdateSignal.connect(preUpdateCallback);
    action->postUpdateSignal.connect(postUpdateCallback);

    while(!engine->windowClosed())
    {
        engine->processEvents();
        engine->updateAllParticleSystems();
        engine->drawAllParticleSystems();
    }

    engine->terminate();
    return 0;
}

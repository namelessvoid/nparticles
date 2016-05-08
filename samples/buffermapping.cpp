/*
 * Copyright (C) 2015 Simon Kerler
 *
 * This file is part of the "Nameless Particle Engine".
 * For conditions of distribution and use, see the copyright notice you should
 * have recievied with this software. I not, see:
 * http://opensource.org/licenses/Zlib
 */

/**
 * This example demonstrates how buffer mapping is used.
 *
 * It creates a particle system with 100 particles rendered as red dots.
 *
 * The positions of these particles are initialised by using buffer mapping.
 * To make the result visible, the camera has to be moved backwards.
 */

#include <iostream>

#include "engine.hpp"
#include "particlesystem.hpp"

int main(int argc, char* argv[])
{
    // Init engine
    nparticles::Engine* engine = nparticles::Engine::getInstance();
    engine->init(1024, 768, false, true);

    // Create shader programs
    nparticles::GPUProgramService* gpuService = engine->getGPUProgramService();
    gpuService->addSourceFile("../res/shader/viewpro-vertex.glsl", "/");
    gpuService->addSourceFile("../res/shader/simple-fragment.glsl", "/");

    gpuService->createRenderProgram("simple-renderer", "/viewpro-vertex.glsl", "/simple-fragment.glsl");

    // Create material
    const nparticles::Material* material = engine->getMaterialManager()->createMaterial("simple-material", "simple-renderer", nparticles::NP_RT_POINTS);

    // Create the particle system
    nparticles::ParticleSystem* pSys = engine->createParticleSystem(100, *engine->getMeshManager()->getDefaultMesh(), *material);

    nparticles::Buffer<glm::vec4>* buffer = pSys->addParticleAttribute<glm::vec4>("b_position");

    glm::vec4* bufferData = buffer->map();
    std::cerr << bufferData << std::endl;

    for(uint i = 0; i < pSys->getParticleCount(); ++i)
        bufferData[i] = glm::vec4(i, 0, 0, 1);

    bufferData = nullptr;
    buffer->unmap();

    while(!engine->windowClosed())
    {
        engine->processEvents();
        engine->updateAllParticleSystems();
        engine->drawAllParticleSystems();
    }

    engine->terminate();
    return 0;
}

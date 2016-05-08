/*
 * Copyright (C) 2015 Simon Kerler
 *
 * This file is part of the "Nameless Particle Engine".
 * For conditions of distribution and use, see the copyright notice you should
 * have recievied with this software. I not, see:
 * http://opensource.org/licenses/Zlib
 */

/**
 * This application was introduced to verify
 * the implementation of the default resoruces.
 *
 * A particle system with one particle is created that uses
 * default resources for mesh and material.
 *
 * Result is a single ret dot that is rendered at the origin of the
 * simulation (camera has to be moved backwards to make it visible).
 */

#include "engine.hpp"
#include "meshmanager.hpp"
#include "materialmanager.hpp"

int main(int argc, char* argv[])
{
    nparticles::Engine* engine = nparticles::Engine::getInstance();
    engine->init(512, 512, false, true);

    engine->createParticleSystem(1, *nparticles::MeshManager::getInstance()->getDefaultMesh(), *nparticles::MaterialManager::getInstance()->getDefaultMaterial());

    while(!engine->windowClosed())
    {
        engine->drawAllParticleSystems();
        engine->processEvents();
    }

    return 0;
}

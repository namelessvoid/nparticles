/*
 * Copyright (C) 2015 Simon Kerler
 *
 * This file is part of the "Nameless Particle Engine".
 * For conditions of distribution and use, see the copyright notice you should
 * have recievied with this software. I not, see:
 * http://opensource.org/licenses/Zlib
 */

#include "computesystem.hpp"

#include "particlesystem.hpp"
#include "action.hpp"
#include "computeprogram.hpp"

namespace nparticles
{

void ComputeSystem::updateParticleSystem(ParticleSystem* particleSystem)
{
    for(auto action : particleSystem->getActions())
    {
        mCurrentComputeProgram = &action->getComputeProgram();
        mCurrentComputeProgram->bind();

        bindParticleBuffers(particleSystem, mCurrentComputeProgram);

        // Invoke pre update signal
        action->preUpdateSignal.emit(particleSystem, this);

        // Activate subroutines. Dot this after the preUpdateSignal so user selected subroutines are activated.
        mCurrentComputeProgram->activateSubroutines();

        // Dispatch enough work groups to cover all particles.
        glDispatchCompute(ceil((float)particleSystem->getParticleCount() / (float)mCurrentComputeProgram->getNumWorkItemsPerGroup()), 1, 1);

        // Synchronise
        glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT | GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT);

        action->postUpdateSignal.emit(particleSystem, this);

    } // for(actions)

    // Disable compute program
    glUseProgram(0);

    unbindParticleBuffers(particleSystem);
}

ComputeSystem::ComputeSystem()
{
}

ComputeSystem::~ComputeSystem()
{
}

} // namespace nparticles

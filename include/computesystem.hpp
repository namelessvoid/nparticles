/*
 * Copyright (C) 2015 Simon Kerler
 *
 * This file is part of the "Nameless Particle Engine".
 * For conditions of distribution and use, see the copyright notice you should
 * have recievied with this software. I not, see:
 * http://opensource.org/licenses/Zlib
 */

#ifndef NP_COMPUTESYSTEM_HPP
#define NP_COMPUTESYSTEM_HPP

#include "gpusystem.hpp"

namespace nparticles
{

class Engine;
class ParticleSystem;
class ComputeProgram;

/**
 * The ComputeSystem class is used to update ParticleSystem%s.
 *
 * The ComputeSystem update a ParticleSystem%s by invoking its Action%s in sequential order.
 */
class ComputeSystem : public GPUSystem
{
friend class Engine;

public:
    /**
     * Update a ParticleSystem.
     *
     * This method updates a given ParticleSystem. All Action%s of this system are executed in sequential
     * order.
     *
     * Action::preUpdateSignal and Action::postUpdateSignal are issued right before / after a
     * ParticleSystem is / was updated. This allows the user to manually bind stuff like buffers.
     *
     * After a particle system was updated, the memory barriers GL_SHADER_STORAGE_BARRIER and GL_VERTEX_ATTRIB_ARRAY_BARRIER
     * are synchronised. For all custom synchronisation, the Action::postUpdateSignal can be used.
     * @param particleSystem The ParticleSystem which is updated.
     */
    void updateParticleSystem(ParticleSystem* particleSystem);

    /**
     * Get the current ComputeProgram.
     *
     * This method can be used to retrieve the currently active ComputeProgram in a Action::preUdpateSignal or
     * Action::postUpdateSignal callback.
     *
     * @return The currently used ComputeProgram.
     */
    ComputeProgram* getCurrentComputeProgram() const { return mCurrentComputeProgram; }

protected:
    /**
     * The ComputeSystem constructor.
     */
    ComputeSystem();

    /**
     * The ComputeSystem destructor.
     */
    ~ComputeSystem();

private:

    /**
     * The currently used ComputeProgram.
     *
     * This is only set during a updateParticleSystem() call.
     */
    ComputeProgram* mCurrentComputeProgram;

    // Hide copy and assignment operators
    ComputeSystem(const ComputeSystem&) = delete;
    void operator=(const ComputeSystem&) = delete;
};

} // namespace nparticles

#endif // NP_COMPUTESYSTEM_HPP

/*
 * Copyright (C) 2015 Simon Kerler
 *
 * This file is part of the "Nameless Particle Engine".
 * For conditions of distribution and use, see the copyright notice you should
 * have recievied with this software. I not, see:
 * http://opensource.org/licenses/Zlib
 */

#ifndef NP_ACTION_HPP
#define NP_ACTION_HPP

#include "signal.hpp"

namespace nparticles
{

class ParticleSystem;
class ComputeProgram;
class ComputeSystem;

/**
 * The Action class represents the behaviour of a particles in a ParticleSystem.
 *
 * Each ParticleSystem can have one or more Actions attached. Each action has a
 * ComputeProgram wich is used to update all particles within a system.
 *
 * Many chained Actions result in a sequentially executed Action list which allow
 * great flexibility when defining the behaviour of a ParticleSystem.
 */
class Action
{
public:
    /**
     * The Action constructor.
     *
     * Creates a new Action with given ComputeProgram.
     *
     * @param computeProgram The ComputeProgram which is used to update the particles.
     */
    Action(ComputeProgram& computeProgram);

    /**
     * The Action destructor.
     */
    ~Action();

    /**
     * Get the ComputeProgram used by the Action.
     *
     * @return The ComputeProgram.
     */
    ComputeProgram& getComputeProgram() { return mComputeProgram; }

    /**
     * The preUpdateSignal emitted right before the Action is applied.
     *
     * This Signal is emitted right before the Action is applied to the particles of a system. This
     * allows the user to set up custom buffers or stuff which is used by the Action's ComputeProgram.
     *
     * @note To preserve OpenGL state all changes done in a pre update callback should be reverted in
     *       a post update callback.
     */
    Signal<ParticleSystem*, const ComputeSystem*> preUpdateSignal;

    /**
     * The postUpdateSignal emitted after the Action was applied.
     *
     * This Signal is emitted after the Action was applied to the particles of a system. This is useful
     * to revert the changes to the OpenGL context done by a pre update callback.
     *
     * A hook to this signal can also be used to perform additional syncrhonisation if necessaray. Note that
     * the ComputeSystem::updateParticleSystem() does some synchronisation on its own.
     */
    Signal<ParticleSystem*, const ComputeSystem*> postUpdateSignal;

private:
    /**
     * The ComputeProgram used by this Action.
     *
     * This is the ComputeProgram used by this Action to update the particles of a system.
     */
    ComputeProgram& mComputeProgram;

    // Hide copy and assignment operators
    Action(const Action&) = delete;
    void operator=(const Action&) = delete;
};

} // namespace nparticles

#endif // NP_ACTION_HPP

/*
 * Copyright (C) 2015 Simon Kerler
 *
 * This file is part of the "Nameless Particle Engine".
 * For conditions of distribution and use, see the copyright notice you should
 * have recievied with this software. I not, see:
 * http://opensource.org/licenses/Zlib
 */

#ifndef NP_GPUSYSTEM_HPP
#define NP_GPUSYSTEM_HPP

namespace nparticles
{

class ParticleSystem;
class ShaderProgram;

/**
 * The GPUSystem class contains code shared between RenderSystem and ComputeSystem.
 *
 * RenderSystem and ComputeSystem use the same code to bind ParticleSystem particle attributes,
 * atomic counters and uniform buffers. This class is used as a base class for both systems
 * to avoid code duplication.
 *
 * This class cannot be instantiated manually but only by its sub-classes.
 */
class GPUSystem
{
protected:
    /**
     * Protected constructor.
     */
    GPUSystem();

    /**
     * Protected virtual destructor.
     */
    virtual ~GPUSystem();

    /**
     * Bind the particle attributes, atomic counters and uniform buffers of a ParticleSystem.
     *
     * This binds all buffers of a particle system to the specified ShaderProgram.
     *
     * @param particleSystem The ParticleSystem to be bound. All its buffers are bound to the @p shaderProgram.
     * @param shaderProgram The ShaderProgram to which the buffers are bound.
     */
    void bindParticleBuffers(ParticleSystem* particleSystem, ShaderProgram* shaderProgram);

    /**
     * Unbinds all particle attributes, atomic counters and uniform buffers of a ParticleSystem.
     *
     * This revertes the bindings applied by bindParticleBuffers(). Note that the ShaderProgram is not passed as parameter
     * since it does not store the current binding but the buffers of the ParticleSystem do.
     *
     * @param particleSystem The ParticleSystem of which the buffers are unbound.
     */
    void unbindParticleBuffers(ParticleSystem* particleSystem);
};

} // namespace nparticles

#endif // NP_GPUSYSTEM_HPP

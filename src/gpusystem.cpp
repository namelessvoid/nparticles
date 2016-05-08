/*
 * Copyright (C) 2015 Simon Kerler
 *
 * This file is part of the "Nameless Particle Engine".
 * For conditions of distribution and use, see the copyright notice you should
 * have recievied with this software. I not, see:
 * http://opensource.org/licenses/Zlib
 */

#include "gpusystem.hpp"

#include "particlesystem.hpp"
#include "shaderprogram.hpp"

namespace nparticles
{

GPUSystem::GPUSystem()
{
}

GPUSystem::~GPUSystem()
{
}

void GPUSystem::bindParticleBuffers(ParticleSystem* particleSystem, ShaderProgram* shaderProgram)
{
    // Bind particle attributes
    ParticleSystem::particle_attribute_buffers attributeBuffers = particleSystem->getParticleAttributeBuffers();
    for(auto attributeIter : attributeBuffers)
        shaderProgram->bindShaderStorageBuffer(attributeIter.first, attributeIter.second);

    // Bind atomic counters
    ParticleSystem::atomic_counter_buffers atomicCounterBuffers = particleSystem->getAtomicCounterBuffers();
    for(auto atomicCounterIter : atomicCounterBuffers)
        shaderProgram->bindAtomicCounterBuffer(atomicCounterIter.first, atomicCounterIter.second);

    // Bind uniform buffers
    ParticleSystem::uniform_buffers uniformBuffers = particleSystem->getUniformBuffers();
    for(auto uniformIter : uniformBuffers)
        shaderProgram->bindUniformBuffer(uniformIter.first, uniformIter.second);
}

void GPUSystem::unbindParticleBuffers(ParticleSystem* particleSystem)
{
    // Bind particle attributes
    ParticleSystem::particle_attribute_buffers attributeBuffers = particleSystem->getParticleAttributeBuffers();
    for(auto attributeIter : attributeBuffers)
        attributeIter.second->unbind();

    // Bind atomic counters
    ParticleSystem::atomic_counter_buffers atomicCounterBuffers = particleSystem->getAtomicCounterBuffers();
    for(auto atomicCounterIter : atomicCounterBuffers)
        atomicCounterIter.second->unbind();

    // Bind uniform buffers
    ParticleSystem::uniform_buffers uniformBuffers = particleSystem->getUniformBuffers();
    for(auto uniformIter : uniformBuffers)
        uniformIter.second->unbind();
}


} // namespace nparticles

/*
 * Copyright (C) 2015 Simon Kerler
 *
 * This file is part of the "Nameless Particle Engine".
 * For conditions of distribution and use, see the copyright notice you should
 * have recievied with this software. I not, see:
 * http://opensource.org/licenses/Zlib
 */

#include "particlesystem.hpp"

#include "action.hpp"

namespace nparticles
{

bool ParticleSystem::swapParticleAttributes(const std::string& firstAttributeName, const std::string& secondAttributeName)
{
    auto firstAttribute = mParticleAttributeBuffers.find(firstAttributeName);

    if(firstAttribute == mParticleAttributeBuffers.end())
    {
        Logger::getInstance()->logWarning("ParticleSystem: attemp to swap attribute " + firstAttributeName + " which does not exist.");
        return false;
    }

    auto secondAttribute = mParticleAttributeBuffers.find(secondAttributeName);

    if(secondAttribute == mParticleAttributeBuffers.end())
    {
        Logger::getInstance()->logWarning("Particle System: attemp to swap attribute " + secondAttributeName  + " which does not exist.");
        return false;
    }

    std::swap(firstAttribute->second, secondAttribute->second);
    return true;
}

Action* ParticleSystem::appendAction(ComputeProgram& computeProgram)
{
    Action* action = new Action(computeProgram);
    mParticleActions.push_back(action);
    return action;
}

BufferBase* ParticleSystem::getParticleAttributeBuffer(const std::string& name)
{
    if(mParticleAttributeBuffers.find(name) == mParticleAttributeBuffers.end())
        return nullptr;
    return mParticleAttributeBuffers[name];
}

void ParticleSystem::emitPreRenderSignal(RenderSystem* renderSystem)
{
    preRenderSignal.emit(this, renderSystem);
}

void ParticleSystem::emitPostRenderSignal(RenderSystem* renderSystem)
{
    postRenderSignal.emit(this, renderSystem);
}

AtomicCounterBuffer* ParticleSystem::addAtomicCounterBuffer(const std::string& name, int itemCount)
{
    if(mAtomicCounterBuffers.find(name) != mAtomicCounterBuffers.end())
        return nullptr;

    AtomicCounterBuffer* newAtomicCounterBuffer = new AtomicCounterBuffer(itemCount);
    mAtomicCounterBuffers[name] = newAtomicCounterBuffer;
    return newAtomicCounterBuffer;
}

AtomicCounterBuffer* ParticleSystem::getAtomicCounterBuffer(const std::string& name)
{
    auto bufferIter = mAtomicCounterBuffers.find(name);

    if(bufferIter == mAtomicCounterBuffers.end())
        return nullptr;

    return bufferIter->second;
}

BufferBase* ParticleSystem::getUniformBuffer(const std::string& name)
{
    auto bufferIter = mUniformBuffers.find(name);

    if(bufferIter == mUniformBuffers.end())
        return nullptr;

    return bufferIter->second;
}

ParticleSystem::ParticleSystem(int particleCount, const Mesh& mesh, const Material& material)
    : mParticleCount(particleCount),
      mMesh(&mesh),
      mMaterial(&material)
{
}

ParticleSystem::~ParticleSystem()
{
    for(auto attributeIter : mParticleAttributeBuffers)
        delete attributeIter.second;
    mParticleAttributeBuffers.clear();

    for(auto actionIter : mParticleActions)
        delete actionIter;
    mParticleActions.clear();

	for(auto uniformBuffer : mUniformBuffers)
		delete uniformBuffer.second;
	mUniformBuffers.clear();

	for(auto atomicCounterBuffer : mAtomicCounterBuffers)
		delete atomicCounterBuffer.second;
	mAtomicCounterBuffers.clear();
}

} // namespace nparticles

/*
 * Copyright (C) 2015 Simon Kerler
 *
 * This file is part of the "Nameless Particle Engine".
 * For conditions of distribution and use, see the copyright notice you should
 * have recievied with this software. I not, see:
 * http://opensource.org/licenses/Zlib
 */

#ifndef NP_PARTICLESYSTEM_HPP
#define NP_PARTICLESYSTEM_HPP

#include <GL/glew.h>

#include <map>
#include <string>

#include "buffer.hpp"
#include "atomiccounterbuffer.hpp"
#include "uniformbuffer.hpp"
#include "signal.hpp"
#include "action.hpp"

namespace nparticles
{
class RenderSystem;

class Material;
class Mesh;

class ComputeProgram;

/**
 * The ParticleSystem class holds all attributes that define a particle system.
 *
 * A ParticleSystem contains a fixed number of particles.
 *
 * A ParticleSystem consists of a set of particle attributes which are stored in Buffer objects.
 * Each particle is associated with one value of each attribute buffer which formes its properties.
 *
 * For rendering, all particles are uniformly associated with a Mesh and a Material. Multiple ParticleSystems
 * can have different Materials and Meshes, but within one ParticleSystem, all Particles are rendered
 * the same.
 *
 * Particles are updated via Actions which invoce compute shaders on the GPU.
 *
 * @see Mesh, Material, Action
 */
class ParticleSystem
{
friend class Engine;

public:
    /**
     * Get the ParticleSystems Mesh.
     *
     * @return The Mesh used to render the ParticleSystem.
     */
    const Mesh* getMesh() const { return mMesh; }

    /**
     * Get the ParticleSystems Material.
     *
     * @return The Material used to render the ParticleSystem.
     */
    const Material* getMaterial() const { return mMaterial; }

    /**
     * Get the number of particles in the ParticleSystem.
     *
     * This is a fixed size since the attribute Buffers are all fixed size and cannot
     * be reallocated.
     *
     * @return The number of particles in the ParticleSystem.
     */
    unsigned int getParticleCount() const { return mParticleCount; }


    // PARTICLE ATTRIBUTES -----------------------------------

    /**
     * Add a new particle attribute to the particles in this system.
     *
     * This method adds a new particle attribute to all particles in the system. Since
     * particle attributes are stored in Buffers, a new Buffer is created and returned.
     * The returned pointer can be used to initialise the Buffer with data.
     *
     * Each attribute is identified by a name wich can be used to retrieve the
     * Buffer associated with an attribute.
     *
     * @param name Name of the new attribute as string.
     * @param glType The corresponding OpenGL type of the attribute (e.g. GL_FLOAT for floats). If not specified, the
     *               OpenGL type is guessed from the template type T.
     * @param glBaseSize The base size of the type of the attribute (e.g. 4 for glm::vec4's). If not specified, the
     *                   base size is guessed from the template type T.
     * @tparam T The type of the new particle attribute.
     *
     * @return The newly created Buffer which holds the new particle attribute.
     */
    template<typename T>
    Buffer<T>* addParticleAttribute(const std::string& name, GLenum glType = GL_INVALID_VALUE, int glBaseSize = -1);

    /**
     * Typedef for the map of particle attributes.
     *
     * This is a map with attribute names (strings) as keys and pointers to the attribute Buffers as values.
     */
    typedef std::map<std::string, BufferBase*> particle_attribute_buffers;

    /**
     * Get the particle attribute buffers.
     *
     * Get the map of particle attribute buffers.
     *
     * @return Reference to the map of particle attribute Buffers.
     */
    const particle_attribute_buffers& getParticleAttributeBuffers() const { return mParticleAttributeBuffers; }

    /**
     * Get a particle attribute Buffer.
     *
     * Get the particle attribute Buffer named @p name.
     *
     * @param name The name of the particle attribute.
     *
     * @return Pointer to the particle attribute Buffer or null pointer if no attribute with given name exists.
     */
    BufferBase* getParticleAttributeBuffer(const std::string& name);

    /**
     * Swap two particle attributes.
     *
     * This method can be used to swap two particle attributes. This comes in handy when double buffered attributes
     * are needed.
     *
     * @param firstAttributeName The name of an attribute to swap.
     * @param secondAttributeName The name of the other attribute to swap.
     *
     * @return True, if the particles are swapped successfully, or false otherwise.
     */
    bool swapParticleAttributes(const std::string& firstAttributeName, const std::string& secondAttributeName);

    /**
     * Append a new action to the ParticleSystem's action list.
     *
     * Appends a new action to the ParticleSystem's action list. When a ParticleSystem is updates,
     * all actions are executed in sequential order the way they have been appended.
     *
     * @param computeProgram The ComputeProgram used by the Action
     *
     * @return Pointer to the newly created Action.
     */
    Action* appendAction(ComputeProgram& computeProgram);

    /**
     * Typedef for the ParticleSystem's action list.
     *
     * This is basically a vector storing pointers to Actions.
     */
    typedef std::vector<Action*> particle_actions;

    /**
     * Get the list of actions.
     *
     * Get the action list of the ParticleSystem.
     *
     * @return Reference to the action list.
     */
    const particle_actions& getActions() const { return mParticleActions; }

    /**
     * Emit the pre render Signal.
     *
     * This method is called by the RenderSystem to emit the pre render Signal.
     *
     * @param renderSystem The RenderSystem that is rendering the ParticleSystem.
     */
    void emitPreRenderSignal(RenderSystem* renderSystem);

    /**
     * Emit the post render Signal.
     *
     * This method is called by the RenderSystem to emit the post render Signal.
     *
     * @param renderSystem The RenderSystem that is rendering the ParticleSystem.
     */
    void emitPostRenderSignal(RenderSystem* renderSystem);

    /**
     * The pre render signal.
     *
     * This Signal is emitted by the RenderSystem just before the actual rendering of the ParticleSystem.
     * By connection to this Signal, it is possible to bind custom vertex attributes or buffers to the
     * RenderSystem / its active RenderProgram.
     */
    Signal<ParticleSystem*, RenderSystem*> preRenderSignal;

    /**
     * The post render signal.
     *
     * This Signal is emitted by the RenderSystem directly after the rendering of the ParticleSystem.
     * Connect to this signal to reverse buffer bindings and so on done by the pre render Signal.
     *
     * To preserve a consistent OpenGL context state, all context changes made by a pre render callback should
     * be reversed by a callback connected to the post render Signal.
     *
     * custom vertex attributes set up by a pre render callback are automatically revoked.
     *
     * @see RenderSystem::setVertexAttribute()
     */
    Signal<ParticleSystem*, RenderSystem*> postRenderSignal;


    // ATOMIC COUNTERS ---------------------------

    /**
     * Typedef for the map of atomic counters.
     *
     * This is a map with the atomic counter uniform name (std::string) as keys and pointers to the AtomicCounterBuffer%s as values.
     */
    typedef std::map<std::string, AtomicCounterBuffer*> atomic_counter_buffers;

    /**
     * Add a new AtomicCounterBuffer to the particle system.
     *
     * Create and attach a new AtomicCounterBuffer for up to @p itemCount atomic counters to the particle system.
     *
     * Atomic counter have to specified with a mandatory binding layout qualifier in shader programs. To correctly
     * bind this buffer, this @p binding has to be provided here.
     *
     * Similar to particle attributes being identified by their name, the binding point is used as the buffer's ID.
     * This means, only one AtomicCounterBuffer can be used for a single binding point.
     *
     * @param name The name of the new atomic counter buffer.
     * @param itemCount The number of atomic counters and, thus the size of the buffer.
     *
     * @return Pointer to the newly created buffer or nullpointer if there is already a buffer for the given @p binding.
     */
    AtomicCounterBuffer* addAtomicCounterBuffer(const std::string& name, int itemCount = 1);

    /**
     * Get the AtomicCounterBuffer associated with a given binding point.
     *
     * This returns the pointer to an AtomicCounterBuffer. The @p binding is used to identifiy the buffer.
     *
     * @param name The name of the atomic counter buffer defined by addAtomicCounterBuffer().
     *
     * @return The pointer to the AtomicCounterBuffer.
     */
    AtomicCounterBuffer* getAtomicCounterBuffer(const std::string& name);

    /**
     * Get the all AtomicCounterBuffers of this particle system.
     *
     * Get the map of atomic counter buffers.
     *
     * @return Reference to the map of AtomicCounterBuffers.
     */
    inline const atomic_counter_buffers& getAtomicCounterBuffers() { return mAtomicCounterBuffers; }


    // UNIFORM BUFFERS -------------------------
    /**
     * Typedef for the map of uniform buffers.
     *
     * This is a map with names (std::string) as key and a pointer to a UniformBuffer (to be precise: pointer to its BufferBase) as values.
     */
    typedef std::map<std::string, BufferBase*> uniform_buffers;

    /**
     * Add new UniformBuffer.
     *
     * Adds a new UniformBuffer which stores data of type T. The @p name is used to identify the buffer within all uniform buffers
     * of this ParticleSystem. The @p itemCount is mandatory since the size of the uniform buffer cannot be guessed from the particle count
     * as it is done for particle attribute buffers.
     *
     * @param name The name of the new UniformBuffer.
     * @param itemCount The number of items of type T that are stored in the buffer (i.e. the buffer size). Defaults to 1.
     *
     * @return Pointer to the newly created UniformBuffer or nullptr, if the name is already occupied by another uniform buffer.
     */
    template<typename T>
    UniformBuffer<T>* addUniformBuffer(const std::string& name, int itemCount = 1);

    /**
     * Get a UniformBuffer by its name.
     *
     * This returns the UniformBuffer named @p name of this ParticleSystem.
     *
     * The returned pointer has to be casted to the correct type if used for data manipulation (Buffer::map() or Buffer::setData()).
     *
     * @param name The name of a uniform buffer.
     * @return Pointer to the UniformBuffer (i.e. pointer to its BufferBase) or nullptr if no buffer with given name exists.
     */
    BufferBase* getUniformBuffer(const std::string& name);

    /**
     * Get all uniform buffers.
     *
     * This returns the map of all uniform buffers which can be used to iterate all UniformBuffers attached to this ParticleSystem.
     *
     * @return Reference to the buffers which is of type uniform_buffers.
     */
    inline const uniform_buffers& getUniformBuffers() { return mUniformBuffers; }

private:
    /**
     * The ParticleSystem constructor.
     *
     * This creates and initialises a ParticleSystem.
     *
     * @note The @p workGroupSizeX,  @p workGroupSizeY and @p workGroupSizeZ parameters must match
     * to the work group sizes specified in compute shaders of Actions used to update this ParticleSystem.
     *
     * @param particleCount The number of particles in the system.
     * @param mesh The Mesh used to render the particles.
     * @param material The Material used to render the particles.
     */
    ParticleSystem(int particleCount, const Mesh& mesh, const Material& material);

    /**
     * Destructor for ParticleSystem.
     */
    ~ParticleSystem();

    /**
     * The particle attribute buffers.
     */
    particle_attribute_buffers mParticleAttributeBuffers;

    /**
     * The atomic counter buffers.
     */
    atomic_counter_buffers mAtomicCounterBuffers;

    /**
     * The uniform buffers.
     */
    uniform_buffers mUniformBuffers;

    /**
     * The list of actions applied to the particles.
     */
    particle_actions mParticleActions;

    /**
     * The number of particles in the system.
     */
    unsigned int mParticleCount;

    /**
     * The Mesh used to render the particles.
     */
    const Mesh* mMesh;

    /**
     * The Material used to render the particles.
     */
    const Material* mMaterial;

    // Hide copy constructor and assignment operator
    ParticleSystem(const ParticleSystem&) = delete;
    void operator=(const ParticleSystem&) = delete;
};



// Implementation
template<typename T>
Buffer<T>* ParticleSystem::addParticleAttribute(const std::string& name, GLenum glType, int glBaseSize)
{
    if(mParticleAttributeBuffers.find(name) != mParticleAttributeBuffers.end())
        return nullptr;

    Buffer<T>* attributeBuffer = new Buffer<T>(mParticleCount, glType, glBaseSize, GL_DYNAMIC_READ, GL_SHADER_STORAGE_BUFFER);
    mParticleAttributeBuffers[name] = attributeBuffer;
    return attributeBuffer;
}

template<typename T>
UniformBuffer<T>* ParticleSystem::addUniformBuffer(const std::string& name, int itemCount)
{
    if(mUniformBuffers.find(name) != mUniformBuffers.end())
        return nullptr;

    UniformBuffer<T>* uniformBuffer = new UniformBuffer<T>(itemCount);
    mUniformBuffers[name] = uniformBuffer;
    return uniformBuffer;
}

}

#endif // NP_PARTICLESTYSTEM_HPP

/*
 * Copyright (C) 2015 Simon Kerler
 *
 * This file is part of the "Nameless Particle Engine".
 * For conditions of distribution and use, see the copyright notice you should
 * have recievied with this software. I not, see:
 * http://opensource.org/licenses/Zlib
 */

#ifndef NP_RENDERSYSTEM_HPP
#define NP_RENDERSYSTEM_HPP

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <vector>
#include <string>

#include "gpusystem.hpp"

#include "buffer.hpp"
#include "renderprogram.hpp"

namespace nparticles
{

// Remove when drawMesh() removed.
class Mesh;
// Remove when drawMesh() removed.
class Material;

class ParticleSystem;

/**
 * The RenderSystem class manages the OpenGL context and handles rendering.
 *
 * The RenderSystem contains all methods required for rendering to OpenGL. It is also responsible to
 * initialise an OpenGL context and a window to render to (by using GLFW3).
 */
class RenderSystem : public GPUSystem
{
friend class Engine;

public:
    /**
     * Set the view projection matrix used to do view and projection transformation within a RenderProgram.
     *
     * This is most likely retrieved by a Camera object and set to the Engine.
     * When rendering, this matrix is passed to the RenderProgram so it this program can
     * performe view and projection transformation.
     *
     * @param viewProjectionMatrix The combined view projection matrix.
     */
    void setViewProjectionMatrix(const glm::mat4& viewProjectionMatrix);

    /**
     * Set the normal matrix used to do normal transformation within a RenderProgram.
     *
     * This is most likely retrieved by a Camera object and set by the Engine. This Matrix is passed on to the
     * RenderProgram so it can be used for normal transformations.
     *
     * @param normalMatrix The normal matrix.
     */
    void setNormalMatrix(const glm::mat3& normalMatrix);

    /**
     * Draw a ParticleSystem.
     *
     * This sets up OpenGL to render a ParticleSystem. It activates the Mesh and Material
     * associated with the ParticleSystem and sets up all vertrex attributes and buffers.
     * It also passes the current view projection matrix to the RenderProgram.3
     *
     * Before and after the actual render call, the ParticleSystem::preRenderSignal and
     * ParticleSystem::postRenderSignal are emitted respectively so you can hook into the
     * rendering process and manually set up stuff.
     *
     * @param particleSystem Pointer to a particle system which should be rendered.
     */
    void drawParticleSystem(ParticleSystem* particleSytem);

    /**
     * Set up a vertex attribute.
     *
     * This method sets up a Buffer as a vertex attribute. This uses the current bound RenderProgram to query the
     * attribute named @p attributeName. If this attribute exists, Buffer @p buffer is set up to provide
     * data for this vertex attribute.
     *
     * @note Only Buffers of types that provide a valid glutils::3GlTypeInfo are allowed!
     *
     * Since multiple ParticleSystems can share one Mesh and Material, the vertex attribute bindings are revoked after the rendering took
     * place. This means, that each rendering call all vertex attributes have to be set. Since they are autoamtically removed, the user
     * does not need to disable any vertex attributes set in a ParticleSystem::preRenderSignal callback.
     *
     * This method is part of the RenderSystem rather than the RenderProgram, since setting vertex attributes in OpenGL
     * changes the state of a vertex array object (VAO). This VAO is part of the Mesh and a RenderProgram should not be
     * responsible to track Mesh states.
     *
     * @note You should never call this function outside a ParticleSystem::preRenderSignal or ParticleSystem::postRenderSignal,
     *       since the Mesh binding and active RenderProgram is only set during a call to drawParticleSystem(). Outside this events
     *       behaviour of calls to this method are undefined!
     *
     * @param attributeName The name of the vertex attribute in a ShaderProgram.
     * @param buffer The Buffer which should provide data for the vertex attribute.
     * @param instanced True if the buffer provides per instance data or false if buffer provides per vertex data. Set to false
     *        (buffer contains per vertex data) by default.
     *
     * @return True, if vertex attribute was set up successfully. False otherwise, i.e. there is no vertex attribute with name
     *         @p attributeName in the ShaderProgram or the type of Buffer does not provide a valid glutils::GlTypeInfo.
     *
     * @tparam T The type of the Buffer.
     *
     * @see GlTypeglutils::GlTypeInfo, Buffer
     */
    template<typename T>
    bool setVertexAttribute(const std::string& attributeName, Buffer<T>* buffer, bool instanced = false);

    /**
     * Get the current bound RenderProgram.
     *
     * Use this method inside a ParticleSystem::preRenderSignal or ParticleSystem::postRenderSignal to get
     * access to the currently bound RenderProgram.
     *
     * @return Pointer to the currently bound RenderProgram.
     */
    RenderProgram* getCurrentRenderProgram() const { return mCurrentRenderProgram; }

    /**
     * Request a new frame.
     *
     * Request a blank frame to render to. This clears the color and depths buffers and initialises
     * an empty frame.
     *
     * This is called by the Engine::drawAllParticleSystems() and should not be called manually!
     */
    void beginFrame();

    /**
     * Request the end of a frame.
     *
     * This method finishes a frame and swaps buffers, so the frame is actually displayed.
     *
     * This is called by the Engine::drawAllParticleSystems() and schould not be called manually!
     */
    void endFrame();

protected:
    /**
     * RenderSystem constructor.
     *
     * You should not create your own RenderSystem but rely on the RenderSystem provided by the Engine.
     */
    RenderSystem();

    /**
     * RenderSystem destructor.
     */
    ~RenderSystem();

    /**
     * Initialises the RenderSystem
     *
     * This method is called by Engine::init() and should never be called manually!
     *
     * It initialises the OpenGL context, OpenGL extensions and a GLFW window for rendering.
     *
     * @param width The width of the window.
     * @param height The height of the window.
     * @param fullscreen Set to true, if you want to enter fullscreen mode.
     *
     * @return On success, a pointer to a GLFWwindow is returned, a null pointer when initialisation failed.
     */
    GLFWwindow* init(int width, int height, bool fullscreen = false, bool debug = false);

    /**
     * Terminates the RenderSystem.
     *
     * This method terminates the RenderSystem, i.e. it shuts down OpenGL context and the window.
     *
     * This methos is called by Engine::terminate() and should nto be called manually!
     */
    void terminate();


    /**
     * Enable / disable V-Sync.
     *
     * Used to control V-Sync behaviour.
     *
     * @note This is overwritten when calling init() so call it after initialisation.
     *
     * @param vSync Set to true to enable V-Sync. False disables V-Sync.
     */
    void useVSync(bool vSync = true);

    /**
     * Enable / disable depth test.
     *
     * Used to control depth test behaviour.
     *
     * @note This is overwritten when calling init() so call it after initialisation.
     *
     * @param depthTest Set to true to enable depth test. False disables depth test.
     */
    void useDepthTest(bool depthTest = true);

private:
    // Hide copy constructor and assignment operator
    RenderSystem(const RenderSystem&) = delete;
    void operator=(const RenderSystem&) = delete;

    /**
     * The glfw window handle.
     *
     * This is a pointer to a GLFWwindow created on RenderSystem::init().
     */
    GLFWwindow* mWindow;

    /**
     * This vector keeps track of the current vertex attribute bindings.
     *
     * The vertex attribute bindings of the current drawParticleSystem() call are stored here.
     * After the drawing, all stored vertex attribute bindings are reset.
     */
    std::vector<GLuint> mVertexAttributeBindings;

    /**
     * This is the currently bound RenderProgram.
     *
     * This is the currently bound RenderProgram of the current drawParticleSystem() call.
     * It can be retrieved via RenderSystem::getCurrentRenderProgram().
     */
    RenderProgram* mCurrentRenderProgram;

    /**
     * The currently used view projection matrix.
     *
     * This can be set by RenderSystem::setViewProjectionMatrix().
     */
    glm::mat4 mViewProjectionMatrix;

    /**
     * The currently used normal matrix.
     *
     * This can be set by RenderSystem::setNormalMatrix().
     */
    glm::mat3 mNormalMatrix;
};

template<typename T>
bool RenderSystem::setVertexAttribute(const std::string& attributeName, Buffer<T>* buffer, bool instanced)
{
    GLint location = mCurrentRenderProgram->getVertexAttributeLocation(attributeName);

    if(location == -1)
    {
        Logger::getInstance()->logWarning("RenderSystem: attemp to set vertex attribute \"" + attributeName + "\" which does not exist!");
        return false;
    }

    if(buffer->getGlType() == GL_INVALID_VALUE || buffer->getGlBaseSize() == -1)
    {
        Logger::getInstance()->logError("RenderSystem: cannot bind vertex attribute for \"" + attributeName + "\" since target buffer does not provide legal GlTypeInfo!");
        return false;
    }
    // Bind buffer so vertex attribute can be set up
    buffer->bind(GL_ARRAY_BUFFER);

    // Set up the vertex attribute pointer and store it
    glVertexAttribPointer(location, buffer->getGlBaseSize(), buffer->getGlType(), GL_FALSE, 0, nullptr);

    if(instanced)
        glVertexAttribDivisor(location, 1);

    glEnableVertexAttribArray(location);
    mVertexAttributeBindings.push_back(location);

    // Release buffer binding to restore state
    buffer->unbind();

    return true;
}

} // namespace nparticles

#endif // NP_RENDERSYSTEM_HPP

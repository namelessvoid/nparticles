/*
 * Copyright (C) 2015 Simon Kerler
 *
 * This file is part of the "Nameless Particle Engine".
 * For conditions of distribution and use, see the copyright notice you should
 * have recievied with this software. I not, see:
 * http://opensource.org/licenses/Zlib
 */

#ifndef NP_ENGINE_HPP
#define NP_ENGINE_HPP

#include <set>
#include <glm/glm.hpp>

#include "computesystem.hpp"
#include "rendersystem.hpp"

#include "meshmanager.hpp"
#include "materialmanager.hpp"

#include "singleton.hpp"
#include "gpuprogramservice.hpp"
#include "camera.hpp"

#include "signal.hpp"

class GLFWwindow;

/**
 * The nparticles namespace is the main namespace which contains the whole simulation engine.
 */
namespace nparticles
{

class ParticleSystem;

/**
 * The Engine class is the main part of the simulation.
 *
 * The Engine is used to manage everything that belongs to a simulation (ParticleSystem%s, RenderSystem and ComputeSytem, etc.). It
 * is a Singleton since only one Engine may be active.
 *
 * Before performing any work, the Engine has to be initialised via init(). When the application exits, terminate() has to be called
 * to guarantee proper resource deallocation.
 *
 * The Engine is used to create new ParticleSystem%s, update them via updateAllParticleSystems() and for rendering via drawAllParticleSystems().
 *
 * Event processing for mouse and keyboard input is pervormed by calling processEvents().
 *
 * The Engine provides all methods required to create a render / "game" loop. A basic implementation would look like this:
 *
 * @code
 * Engine* engine = Engine::getInstance();
 * engine->init(512, 512, false);
 *
 * while(!engine->windowClosed())
 * {
 *     engine->processEvents();
 *     engine->updateAllParticleSystems();
 *     engine->drawAllParticleSystems();
 * }
 * @endcode
 *
 *
 */
class Engine : public Singleton<Engine>
{
friend class Singleton<Engine>;

public:
    /**
     * Initialise the Engine.
     *
     * The Engine is initalised. A window for rendering and an OpenGL context are created.
     *
     * @note This method has to be called before any other methods are invoked!
     *
     * @param width The width of the render window.
     * @param height The height of the render window.
     * @param fullscreen Set to true if render window should be fullscreen. False by default.
     */
    void init(int width, int height, bool fullscreen = false, bool debug = false);

    /**
     * Shut down the Engine.
     *
     * This method should be called when the Engine is no longer needed. It frees
     * the render window and releases the OpenGL context.
     *
     * @note No update, draw or any other methods
     * must be invoked afterwards!
     */
    void terminate();

    /**
     * @copydoc RenderSystem::useVSync()
     */
    inline void useVSync(bool vSync = true) { mRenderSystem.useVSync(vSync); }

    /**
     * @copydoc RenderSystem::useDepthTest()
     */
    inline void useDepthTest(bool depthTest = true) { mRenderSystem.useDepthTest(depthTest); }

    /**
     * Update all ParticleSystem%s.
     *
     * This method can be called to update all ParticleSystem%s of the Engine. All systems are
     * updated sequentially by invoking their respective Action%s.
     *
     * A good idea to call this method is at the beginning of a render / "game" loop.
     */
    void updateAllParticleSystems();

    /**
     * Render all ParticleSystem%s.
     *
     * This method renders all ParticleSystem%s sequentially.
     *
     * A good idea to call this method is at the end of a render / "game" loop, i.e. after updateAllParticleSystems() was called.
     *
     * @note Since this method calls RenderSystem::beginFrame() and RenderSystem::endFrame(), all manually drawn stuff is whiped out!
     *
     * @todo Check if this is an issue for complex simulations.
     */
    void drawAllParticleSystems();

    /**
     * Process all input events.
     *
     * This method processes all user keyboard and mouse events.
     *
     * @note You have to call this method within your render / "game" loop or all input events are ignored!
     */
    void processEvents();

    /**
     * The keyEventSignal emitted when a key was pressed.
     *
     * Connect to this signal to create custom key handlers. This dies *not* overwrite the default key handler.
     *
     * Parameters are (order left to right): the GLFW key code, the action (GLFW_PRESSED | GLFW_RELEASED) and a bit flag of which modifier keys (shift etc.) are
     * pressed.
     *
     * @see http://www.glfw.org/docs/latest/group__input.html
     */
    Signal<int, int, int> keyEventSignal;

    /**
     * Check if the render window was closed.
     *
     * This evaluates to true if the render window was closed. It can be used to check when a render / "game" loop should
     * terminated:
     *
     * @code
     * while(!engine->windowClosed())
     * {
     *     // do stuff
     * }
     * @endcode
     *
     * @return True if the render window was closed, false otherwise.
     */
    bool windowClosed();

    /**
     * Create and register a new ParticleSystem.
     *
     * Use this method to create a new ParticleSystem registered to the Engine. This ParticleSystem can now be
     * updated via updateAllParticleSystems() and rendered via drawAllParticleSystems().
     *
     * @param particleCount The number of particles in the ParticleSystem.
     * @param mesh The Mesh used to render the particles.
     * @param material The Material used to render the particles.
     *
     * @return Pointer to the newly created ParticleSytem.
     */
    ParticleSystem* createParticleSystem(int particleCount, const Mesh& mesh, const Material& material);

    /**
     * Delete a ParticleSystem.
     *
     * Removes a ParticleSystem from the Engine and deletes it. All pointers referring to ParticleSystem data
     * (buffers, actions, pointer to particle system itself) become invalid!
     *
     * @param particleSystem The ParticleSystem to delete.
     */
    void deleteParticleSystem(ParticleSystem* particleSystem);

    /**
     * Get handle for GPUProgramService.
     *
     * Get a pointer to the GPUProgramService. This is useful to load shader source files. For details see GPUProgramService.
     *
     * @return Pointer to the GPUPRogramService.
     */
    inline GPUProgramService* getGPUProgramService() { return &mGPUProgramService; }

    /**
     * Get the MeshManager.
     *
     * This is a convenience method which returns the MeshManager singleton instance. This is equivalent to
     *
     * @code
     * MeshManager::getInstance();
     * @endcode
     *
     * @return Pointer to the MeshManager.
     */
    inline MeshManager* getMeshManager() const { return MeshManager::getInstance(); }

    /**
     * Get the MaterialManager.
     *
     * This is a convenience method which returns the MaterialManager singleton instance. This is equivalent to
     *
     * @code
     * MaterialManager::getInstance();
     * @endcode
     *
     * @return Pointer to the MaterialManager.
     */
    inline MaterialManager* getMaterialManager() const { return MaterialManager::getInstance(); }

    /**
     * Set the OpenGL debug level.
     *
     * This sets the severity level used by the OpenGL debug routine. All values of GL_DEBUG_SEVERITY_* are accepted.
     *
     * @param level The new debug level.
     */
    inline void setGlDebugLevel(GLenum level) { glutils::setGlDebugLevel(level); }

    /**
     * Get the Camera of the Engine.
     *
     * Get access to the Camera object used to view the scene created by the Engine.
     *
     * @return The Camera.
     */
    inline Camera& getCamera() { return mCamera; }

    /**
     * Get the RenderSystem of the Engine.
     *
     * Get the RenderSystem to have more control over the render flow.
     *
     * @return Reference to the RenderSystem.
     */
    inline RenderSystem& getRenderSystem() { return mRenderSystem; }

private:
    /**
     * Private Engine constructor.
     */
    Engine();

    /**
     * Private Engine destructor.
     */
    ~Engine();

    // Input callbacks

    /**
     * The static key callback registered to GLFW3.
     *
     * This is a static key callback that can be registered to GLFW3. This is needed since no class methods can be
     * registered via glfwSetKeyCallback().
     *
     * Internally, Engine::keyCallback() is called.
     *
     * For full details see the GLFW3 documentation.
     *
     * @param window The window for which the event was received.
     * @param key The GLFW key code.
     * @param scancode A sytem dependant code for the key.
     * @param action One of GLFW_PRESS or GLFW_RELEASE.
     * @param modifier Bitfield of modifier keys.
     */
    static void staticKeyCallback(GLFWwindow* window, int key, int scancode, int action, int modifier);

    /**
     * The key callback function.
     *
     * This method responds to keyboard input. In the current implementation, only the Camera movement is updated.
     *
     * @param window The window on which a key event occured.
     * @param key The GLFW key code.
     * @param action One of GLFW_PRESS or GLFW_RELEASE.
     * @param modifier Bitfield of modifier keys.
     */
    void keyCallback(GLFWwindow* window, int key, int action, int modifier);

    /**
     * The static cursor position callback registered to GLFW3.
     *
     * This is a static cursor position callback that can be registered to GLFW3. This is needed since no class methods
     * can be registered via glfwSetCursorPosCallback().
     *
     * Internally, Engine::cursorPosCallback() is called.
     *
     * For full details see the GLFW3 documentation.
     *
     * @param window The window on which the event occured.
     * @param xpos The new x position of the mouse cursor.
     * @param ypos The new y pposition of the mouse cursor.
     */
    static void staticCurserPosCallback(GLFWwindow* window, double xpos, double ypos);

    /**
     * The cursor position callback function.
     *
     * This method responds to mouse moves. In the current implementation, only the Camera direction is updated.
     *
     * @param window The window on which the mouse move occured.
     * @param xpos The new x position of the mouse cursor.
     * @param ypos The new y position of the mouse cursor.
     */
    void cursorPosCallback(GLFWwindow* window, double xpos, double ypos);

    /**
     * The static mouse button callback registered to GLFW3.
     *
     * This is a static mouse button callback that can be registered to GLFW3. This is needed since no class methods
     * can be registered via glfwSetMouseButtonCallback().
     *
     * Internally, Engine::mouseButtonCallback() is called.
     *
     * For full details see the GLFW3 documentation.
     *
     * @param window The window on which the mouse button event occured.
     * @param button The ID of the button e.g. GLFW_BUTTON_1 for a left click.
     * @param action One of GLFW_PRESS or GLFW_RELEASE.
     * @param modifier Bitfield of modifier keys.
     */
    static void staticMouseButtonCallback(GLFWwindow* window, int button, int action, int modifier);

    /**
     * The mouse button callback function.
     *
     * This method responds to mouse button events.
     *
     * @param window The window on which the mouse button event occured.
     * @param button The ID of the button e.g. GLFW_BUTTON_1 for a left click.
     * @param action One of GLFW_PRESS or GLFW_RELEASE.
     * @param modifier Bitfield of modifier keys.
     */
    void mouseButtonCallback(GLFWwindow* window, int button, int action, int modifier);

    // Attributes

    /**
     * The main Camera used.
     *
     * The main Camera used for view and projection transformations.
     */
    Camera mCamera;

    /**
     * The last known mouse cursor position.
     *
     * Used to compute the direction change for the Camera.
     */
    glm::dvec2 mLastCursorPosition;

    /**
     * The ComputeSystem to update ParticleSystems.
     */
    ComputeSystem mComputeSystem;

    /**
     * The RenderSystem to render Particle Systems.
     */
    RenderSystem mRenderSystem;

    /**
     * The GLFWwindow used for rendering and event processing.
     */
    GLFWwindow* mWindow;

    /**
     * The GPUProgramService to manage ShaderProgram%s.
     */
    GPUProgramService mGPUProgramService;

    /**
     * All created and managed ParticleSystem%s.
     */
    std::set<ParticleSystem*> mParticleSystems;

    // Hide copy constructor and assignment operators
    Engine(const Engine&) = delete;
    void operator=(const Engine&) = delete;
};

} // namespace nparticles

#endif // NP_ENGINE_HPP

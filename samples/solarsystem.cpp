/*
 * Copyright (C) 2015 Simon Kerler
 *
 * This file is part of the "Nameless Particle Engine".
 * For conditions of distribution and use, see the copyright notice you should
 * have recievied with this software. I not, see:
 * http://opensource.org/licenses/Zlib
 */


/**
 * This simulation simulates the movement of the 8 planets of our solar system
 * and the dwarf planet Pluto.
 *
 * It uses scientific data to initialise planets. These are rendered as tessellated
 * icosahedra in wireframe mode.
 *
 * Controls:
 * - wasd: foreward, strafe left, backward, strafe right
 * - xy: rotate counter clockwise / clockwise
 * - qe: descend / ascend
 * - LMB pressed + mouse movement: rotate camera
 * - Space: toggle pause mode
 * - v: toggle v-sync
 * - t: draw trajectories (camera should not be moved in this mode to avoid visual artifacts!)
 **/

#include "engine.hpp"
#include "particlesystem.hpp"

using namespace nparticles;

/**
 * Enum of planets contained in this simulation.
 */
enum Planets
{
    SUN,
    MERCURY,
    VENUS,
    EARTH,
    MARS,
    JUPITER,
    SATURN,
    URANUS,
    NEPTUNE,
    PLUTO,
    PLANET_COUNT
};


/**
 * Initialise the simulation, i.e. its particles
 *
 * @param position Pointer to a memory location storing planet positions.
 * @param velocities Pointer to a memory location storing planet velocities.
 * @param colors Pointer to a memory location storing planet colors.
 */
void initSolarSystem(glm::dvec4* positions, glm::dvec4* velocities, glm::vec4* colors)
{
    // Data taken from
    // http://nssdc.gsfc.nasa.gov/planetary/factsheet/sunfact.html
    // http://nssdc.gsfc.nasa.gov/planetary/factsheet/
    positions[SUN] = glm::dvec4(0, 0, 0, 1.988500e30);
    velocities[SUN] = glm::dvec4(0, 0, 0, 100);
    colors[SUN] = glm::vec4(1, 0.5, 0, 1);

    positions[MERCURY] = glm::dvec4(57.9e9, 0, 0, 0.33e24);
    velocities[MERCURY] = glm::dvec4(0, 47.4e3, 0, 1);
    colors[MERCURY] = glm::vec4(0.5, 0.5, 0.6, 1.0);

    positions[VENUS] = glm::dvec4(108.2e9, 0, 0, 4.87e24);
    velocities[VENUS] = glm::dvec4(0, 35.0e3, 0, 1.1);
    colors[VENUS] = glm::vec4(0.9, 1.0, 0.8, 1.0);

    positions[EARTH] = glm::dvec4(149.6e9, 0, 0, 5.97e24);
    velocities[EARTH] = glm::dvec4(0, 29.8e3, 0, 3);
    colors[EARTH] = glm::vec4(0.5, 0.5, 1.0, 1.0);

    positions[MARS] = glm::dvec4(227.9e9, 0, 0, 0.642e21);
    velocities[MARS] = glm::dvec4(0, 24.1e3, 0, 1.5);
    colors[MARS] = glm::vec4(1, 0.1, 0.1,1 );

    positions[JUPITER] = glm::dvec4(778.6e9, 0, 0, 1898e24);
    velocities[JUPITER] = glm::dvec4(0, 13.1e3, 0, 30);
    colors[JUPITER] = glm::vec4(0.8, 0.8, 0, 1);

    positions[SATURN] = glm::dvec4(1433.5e9, 0, 0, 568e24);
    velocities[SATURN] = glm::dvec4(0, 9.7e3, 0, 25);
    colors[SATURN] = glm::vec4(1, 0.8, 0.8, 1);

    positions[URANUS] = glm::dvec4(2872.5e9, 0, 0, 86.8e24);
    velocities[URANUS] = glm::dvec4(0, 6.8e3, 0, 20);
    colors[URANUS] = glm::vec4(0.6, 0.8, 1, 1);

    positions[NEPTUNE] = glm::dvec4(4495.1e9, 0, 0, 102e24);
    velocities[NEPTUNE] = glm::dvec4(0, 5.4e3, 0, 25);
    colors[NEPTUNE] = glm::vec4(0.3, 0.6, 1, 0);

    positions[PLUTO] = glm::dvec4(5870e9, 0, 0, 0.0132e24);
    velocities[PLUTO] = glm::dvec4(0, 4.7e3, 0, 2);
    colors[PLUTO] = glm::vec4(0.8, 0.7, 0.5, 1);
}

// Control pause mode
bool paused = true;
bool vSync = true;
bool glClearDisabled = false;

/**
 * Keylistener for special input controls.
 *
 * Space: Toggle pause mode
 * v: Toggle v-sync
 * t: Toggle trajectories (i.e. do not call RenderSystem::beginFrame())
 */
void keyListener(int keycode, int action, int modifiers)
{
    if(action != GLFW_PRESS)
        return;

    // Toggle pause and vsync
    if(keycode == GLFW_KEY_SPACE)
        paused = !paused;
    else if(keycode == GLFW_KEY_V)
    {
        vSync = !vSync;
        Engine::getInstance()->useVSync(vSync);
    }
    else if(keycode == GLFW_KEY_T)
        glClearDisabled = !glClearDisabled;
}

/**
 * Main method setting up and running the simulation.
  */
int main()
{
    Engine* engine = Engine::getInstance();
    engine->init(1440, 900, false);
    engine->keyEventSignal.connect(keyListener);

    // Move camera backwards to make planets visible.
    engine->getCamera().move(glm::vec3(0, 0, -3000));

    // Since no lighting is performed, draw wire frames for better visualisation.
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // Set up shaders
    GPUProgramService* gpuProgramService = engine->getGPUProgramService();
    gpuProgramService->addSourceDirectory("../res/shader/solarsystem", "/solarsystem");
    gpuProgramService->addSourceDirectory("../res/shader/np", "/np");

    if(!gpuProgramService->createRenderProgram("renderer",
                                               "/solarsystem/vertex.glsl",
                                               "/solarsystem/fragment.glsl",
                                               "/solarsystem/tessellationcontrol.glsl",
                                               "/solarsystem/tesselationevaluation.glsl")
            ) // if(render program creation failed)
        return -1;

    if(!gpuProgramService->createComputeProgram("updater", "/solarsystem/update.glsl"))
        return -1;

    // Set up mesh and material
    const Material* material = engine->getMaterialManager()->createMaterial("material", "renderer");
    const Mesh* mesh = engine->getMeshManager()->createIcosahedron("icosaeder");

    // Set up the particle system.
    ParticleSystem* pSys = engine->createParticleSystem(PLANET_COUNT, *mesh, *material);
    UniformBuffer<glm::vec4>* colorBuffer = pSys->addUniformBuffer<glm::vec4>("Colors", PLANET_COUNT);
    auto positionBuffer = pSys->addParticleAttribute<glm::dvec4>("Positions");
    auto velocityBuffer = pSys->addParticleAttribute<glm::dvec4>("Velocities");

    pSys->appendAction(*gpuProgramService->getComputeProgram("updater"));

    // Initialise buffers of the particle system.
    initSolarSystem(positionBuffer->map(), velocityBuffer->map(), colorBuffer->map());
    positionBuffer->unmap();
    velocityBuffer->unmap();
    colorBuffer->unmap();

    RenderSystem& renderSystem = engine->getRenderSystem();

    // Main loop
    while(!engine->windowClosed())
    {
        engine->processEvents();

        if(!paused)
            engine->updateAllParticleSystems();

        // Omit the call to glClear() to draw trajectories.
        if(!glClearDisabled)
            renderSystem.beginFrame();

        // Manually updating the position and setting the view projection matrix.
        // This is usually performed by Engine::drawAllParticleSystems().
        engine->getCamera().updatePosition();
        renderSystem.setViewProjectionMatrix(engine->getCamera().getViewProjectionMatrix());

        // Render particles and swap buffers.
        renderSystem.drawParticleSystem(pSys);
        renderSystem.endFrame();
    }

    engine->terminate();
    return 0;
}


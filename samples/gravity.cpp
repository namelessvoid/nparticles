/*
 * Copyright (C) 2015 Simon Kerler
 *
 * This file is part of the "Nameless Particle Engine".
 * For conditions of distribution and use, see the copyright notice you should
 * have recievied with this software. I not, see:
 * http://opensource.org/licenses/Zlib
 */

/**
 * This sample contains a particle system with gravitation
 * based particles as well as an update benchmark.
 *
 * There are different integration types and optimizations provided to udpate the particles.
 * By passing command line switches, the desired type can be selected:
 * - -euler-no-shared:          Improved euler integration without shared memory optimization.
 * - -euler-shared:             Improved euler integration with shared memory optimization.
 * - -verlet-no-shared:         Verlet integration withouth shared memory optimization.
 * - -verlet-shared:            Verlet integration with shared memory optimization.
 * - -verlet-shared-buffering:  Verlet integration with shared memory optimization and client-side double buffering.
 *
 * # Interactive simulation
 *
 * Controls:
 * - Mouse to look around.
 * - wasd to move
 * - yx to rotate
 * - Space to toggle pause
 * - nm to reduce / increase time step
 * - c toggles quad / circle shaped particles
 *
 * Color modes selected through number keys:
 * - 1: velocity vector
 * - 2: speed (length of velocity vector)
 * - 3: distance to (0, 0, 0)
 * - 4: plain yellow
 *
 * The FPS per frame are logged to standard output.
 *
 *
 * # Benchmark
 *
 * The benchmark subsequetially updates particle systems with different particle counts.
 * Each configuration is updated 100 times to increase accuracy.
 * For integration, the integration method and optimisation technique selected on command
 * line is used (see above).
 *
 * For each update call, set up, update and clean up processes are profiled. The
 * eastimated GFLOPS are calculated as well.
 **/

#include "engine.hpp"
#include "gpuprogramservice.hpp"
#include "particlesystem.hpp"
#include "buffer.hpp"
#include "rendersystem.hpp"
#include "computesystem.hpp"
#include "computeprogram.hpp"

#include "gpuclock.hpp"
#include "cpuclock.hpp"

#include <iostream>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

using namespace nparticles;

// Structures used to declare the particle system. Not required, if a vec4 is used for both
// positions and velocities.
struct ParticlePosition
{
    glm::vec3 position;
    float mass;
};

// Some simulation control variables
bool paused = true;
float timeStep = 0.001;
float timeStepChangeResolution = 0.0001;

std::string colorFunction = "yellowSubroutine";
std::string spriteType = "circle";

Buffer<GLuint>* atomicCounterBuffer;

// There are several methods that can be used to update the particles. This variable
// stores the name of a shader which is used to update the particles. For details
// see parseCommandLineSwitch() method.
std::string updateShader = "/gravity/update-euler-no-shared.glsl";

enum ParticleIntegrationType
{
    PIT_EULER_NO_SHARED,
    PIT_EULER_SHARED,
    PIT_VERLET_NO_SHARED,
    PIT_VERLET_SHARED,
    PIT_VERLET_SHARED_DOUBLE_BUFFERING
};

ParticleIntegrationType particleIntegrationType = PIT_EULER_NO_SHARED;

// This flag toggles benchmark mode. It can be set via '-benchmark' command line switch.
bool benchmarkMode = false;

uint localWorkGroupSize = 0;

// --------
// Listeners
// --------

// The pre update listener
void preUpdateListener(const ParticleSystem* particleSystem, const ComputeSystem* computeSystem)
{
    ComputeProgram* computeProgram = computeSystem->getCurrentComputeProgram();
    computeProgram->setUniform("timeStep", timeStep);
    computeProgram->setUniform("particleCount", particleSystem->getParticleCount());
}

// post update listener for double buffered Verlet integration.
void postUpdateListener(const ParticleSystem* particleSystem, const ComputeSystem*)
{
    ((ParticleSystem*)particleSystem)->swapParticleAttributes("ParticlePositions", "ParticleProperties");
}

// The pre render listener
void preRenderListener(const ParticleSystem* particleSystem, const RenderSystem* renderSystem)
{
    RenderProgram* renderProgram = renderSystem->getCurrentRenderProgram();
    renderProgram->selectSubroutine(GL_VERTEX_SHADER, "colorFunction", colorFunction);
    renderProgram->selectSubroutine(GL_FRAGMENT_SHADER, "createSpriteShape", spriteType);

    if(particleIntegrationType == PIT_VERLET_NO_SHARED ||
            particleIntegrationType == PIT_VERLET_SHARED ||
            particleIntegrationType == PIT_VERLET_SHARED_DOUBLE_BUFFERING)
    {
        renderProgram->setUniform("usingVerlet", true);
    }
}


// --------
// Key event handler
// --------
void keyEventCallback(int keycode, int action, int modifiers)
{
    if(action != GLFW_PRESS)
        return;

    // Toggle pause mode
    switch(keycode)
    {
    // Toggle pause
    case GLFW_KEY_SPACE:
        paused = !paused;
        break;
    // Decrease time step
    case GLFW_KEY_N:
        timeStep -= timeStepChangeResolution;
        // Prevent time step from becoming negative.
        timeStep = (timeStep > 0)?timeStep:0;
        break;
    // Increase time step
    case GLFW_KEY_M:
        timeStep += timeStepChangeResolution;
        break;
    // Switch fragment color routine
    case GLFW_KEY_1:
        colorFunction = "velocityVector";
        break;
    case GLFW_KEY_2:
        colorFunction = "velocitySpeed";
        break;
    case GLFW_KEY_3:
        colorFunction = "distance";
        break;
    case GLFW_KEY_4:
        colorFunction = "yellowSubroutine";
        break;
    case GLFW_KEY_C:
        if(spriteType == "circle")
            spriteType = "quad";
        else
            spriteType = "circle";
        break;
    }
}

// Create a particle system with given number of particles
ParticleSystem* createParticleSystem(int particleCount);

// Invoke the update benchmark
void updateBenchmark();

// Used to parse command line switches.
void parseCommandLineSwitch(std::string cliSwitch);

// -----------
// Main method
// -----------
int main(int argc, char* argv[])
{
    for(int i = 1; i < argc; ++i)
        parseCommandLineSwitch(argv[i]);

    Engine* engine = Engine::getInstance();
    engine->init(1440, 900, false, false);

    engine->keyEventSignal.connect(keyEventCallback);
    engine->useDepthTest(false);

    // Enable blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE);
    glBlendEquation(GL_FUNC_ADD);

    // Load shader sources
    GPUProgramService* gpuService = engine->getGPUProgramService();
    gpuService->addSourceDirectory("../res/shader/gravity", "/gravity");
    gpuService->addSourceDirectory("../res/shader/np", "/np");

    // Create programs
    if(!gpuService->createRenderProgram("gravity-render", "/gravity/vertex.glsl", "/gravity/fragment.glsl"))
        return -1;

    if(!gpuService->createComputeProgram("gravity-update", updateShader))
        return -1;
    localWorkGroupSize = gpuService->getComputeProgram("gravity-update")->getNumWorkItemsPerGroup();

    // Create material
    MaterialManager* materialManager = engine->getMaterialManager();
    materialManager->createMaterial("gravity-material", "gravity-render", NP_RT_POINTS);

    // Benchmarking
    if(benchmarkMode)
    {
        updateBenchmark();
        engine->terminate();
        exit(0);
    }

    // Create particle system
    ParticleSystem* pSys = createParticleSystem(1200);
    pSys->preRenderSignal.connect(preRenderListener);

    GPUClock clock;

    while(!engine->windowClosed())
    {
        engine->processEvents();

        clock.start();

        if(!paused)
            engine->updateAllParticleSystems();

        engine->drawAllParticleSystems();

        clock.stop();

        glFinish();

        // Print debug counter
        std::cout << 1.0 / clock.getElapsedTime() << std::endl;
    }

    engine->terminate();

    return 0;
}


// ---------------
// Parse command line switches
// ---------------
void parseCommandLineSwitch(std::string cliSwitch)
{
    if(cliSwitch == "-benchmark")
        benchmarkMode = true;
    else if(cliSwitch == "-euler-no-shared")
    {
        particleIntegrationType = PIT_EULER_NO_SHARED;
        updateShader = "/gravity/update-euler-no-shared.glsl";
    }
    else if(cliSwitch == "-euler-shared")
    {
        particleIntegrationType = PIT_EULER_SHARED;
        updateShader = "/gravity/update-euler-shared.glsl";
    }
    else if(cliSwitch == "-verlet-no-shared")
    {
        particleIntegrationType = PIT_VERLET_NO_SHARED;
        updateShader = "/gravity/update-verlet-no-shared.glsl";
    }
    else if(cliSwitch == "-verlet-shared")
    {
        particleIntegrationType = PIT_VERLET_SHARED;
        updateShader = "/gravity/update-verlet-shared.glsl";
    }
    else if(cliSwitch == "-verlet-shared-double-buffer")
    {
        particleIntegrationType = PIT_VERLET_SHARED_DOUBLE_BUFFERING;
        updateShader = "/gravity/update-verlet-shared-double-buffering.glsl";
    }
    else
    {
        std::cerr << "No such command line option: " << cliSwitch << ".\n"
                  << "Available options are:\n"
                  << "    -benchmark\trun benchmark suite\n"
                  << "    -euler-no-shared\tuse improved Euler integration without shared memory\n"
                  << "    -euler-shared\tuse improved Euler integration with shared memory\n"
                  << "    -verlet-no-shared\tuse Verlet integration without shared memory\n"
                  << "    -verlet-shared\tuse Verlet integration with shared memory\n"
                  << "    -verlet-shared-double-buffer\tuse Verlet integration with shared memory and double buffering\n"
                  << "    -help\t\tprint this help text.\n";
        exit(0);
    }
}

// ---------------
//
// Benchmark stuff
//
// ---------------
const int updateBenchmarkParticleCounts[] = {
    2,
    512,
    1024,
    4096,
    8192,
    16384,
    32768,
    65536,
    131072,
    262144
};

// Helper for calculating FLOPS per update call
// Constants are manually determined and hard coded (do not forget to update
// them when the shaders change ;)
u_int64_t getFlopsPerUpdate(int particleCount)
{
    const u_int64_t interactionFlops = 22;

    u_int64_t perParticleFlops;
    if(particleIntegrationType == PIT_EULER_NO_SHARED || particleIntegrationType == PIT_EULER_SHARED)
        perParticleFlops = 18;
    else
        perParticleFlops = 21;
    return particleCount * (perParticleFlops + interactionFlops * particleCount);
}

ParticleSystem* createParticleSystem(int particleCount)
{
    Engine* engine = Engine::getInstance();
    MaterialManager* materialManager = engine->getMaterialManager();
    GPUProgramService* gpuService = engine->getGPUProgramService();

    ParticleSystem* pSys = engine->createParticleSystem(particleCount, *engine->getMeshManager()->getDefaultMesh(), *materialManager->getMaterial("gravity-material"));
    Buffer<ParticlePosition>* pPositions  = pSys->addParticleAttribute<ParticlePosition>("ParticlePositions");
    Buffer<glm::vec4>* pVelocities = pSys->addParticleAttribute<glm::vec4>("ParticleProperties");

    Action* action = pSys->appendAction(*gpuService->getComputeProgram("gravity-update"));

    action->preUpdateSignal.connect(preUpdateListener);

    if(particleIntegrationType == PIT_VERLET_SHARED_DOUBLE_BUFFERING)
        action->postUpdateSignal.connect(postUpdateListener);

    // Provide initial data
    ParticlePosition* pPositionData = pPositions->map();
    glm::vec4* pPropertyData = pVelocities->map();

    srand(time(nullptr));
    for(size_t i = 0; i < pSys->getParticleCount(); ++i)
    {
        // Position
        pPositionData[i].position = glm::vec3(
                    random() % 100 - 50,
                    random() % 100 - 50,
                    random() % 100 - 50);
        // Mass
        pPositionData[i].mass = random() % 200 + 50;

        // Velocity
        pPropertyData[i] = glm::vec4(random() % 50 - 25, random() % 50 - 25, random() % 50 - 25, 0);

        // Encode initial velocity as position if Verlet integration is used
        if(particleIntegrationType == PIT_VERLET_NO_SHARED || particleIntegrationType == PIT_VERLET_SHARED || particleIntegrationType == PIT_VERLET_SHARED_DOUBLE_BUFFERING)
        {
            pPropertyData[i] = glm::vec4(pPositionData[i].position, 0) + pPropertyData[i] * 0.001f;

            // Store mass in positions and properties so it does not get lost for the shader after swapping the buffers
            if(particleIntegrationType == PIT_VERLET_SHARED_DOUBLE_BUFFERING)
                pPropertyData[i][3] = pPositionData[i].mass;
        }
    }

    pPositions->unmap();
    pVelocities->unmap();

    return pSys;
}



// Benchmark clocks
GPUClock gpuClock;
CPUClock cpuClock;
CPUClock fpsClock;

// Time variables
double cpuSetUpTime;
double gpuSetUpTime;
double cpuUpdateTime;
double gpuUpdateTime;
double cpuCleanUpTime;
double gpuCleanUpTime;


// Benchmark listeners
void benchmarkPreUpdateListener(ParticleSystem*, const ComputeSystem*)
{
    cpuClock.stop();
    gpuClock.stop();
    glFinish();

    cpuSetUpTime += cpuClock.getElapsedTime();
    gpuSetUpTime += gpuClock.getElapsedTime();

    gpuClock.start();
    cpuClock.start();
}

void benchmarkPostUpdateListener(ParticleSystem*, const ComputeSystem*)
{
    cpuClock.stop();
    gpuClock.stop();
    glFinish();

    cpuUpdateTime += cpuClock.getElapsedTime();
    gpuUpdateTime += gpuClock.getElapsedTime();

    gpuClock.start();
    cpuClock.start();
}

// Benchmark the update performance
void updateBenchmark()
{
    Engine* engine = Engine::getInstance();

    size_t runsPerConfiguration = 100;

    std::cout << "\n"
              << "+----------------------------+\n"
              << "| Gravity sample - Benchmark |\n"
              << "+----------------------------+\n"
              << "\n";

    std::cout << "Starting benchmark.\nAll times are given in seconds.\n\n";

    // Print integration type and optimisation techniques
    switch(particleIntegrationType)
    {
    case PIT_EULER_NO_SHARED:
        std::cout << "Improved euler, no shared memory";
        break;
    case PIT_EULER_SHARED:
        std::cout << "Improved euler, shared memory";
        break;
    case PIT_VERLET_NO_SHARED:
        std::cout << "Verlet, no shared memory";
        break;
    case PIT_VERLET_SHARED:
        std::cout << "Verlet, shared memory";
        break;
    case PIT_VERLET_SHARED_DOUBLE_BUFFERING:
        std::cout << "Verlet, shared memory, double buffering";
        break;
    }
    std::cout << ", local work group size: " << localWorkGroupSize << "\n";

    std::cout << "# particles\tCPU set up time\tGPU set up\tCPU update\tGPU update\tCPU clean up\tGPU clean up\tTotal time\test. FPS\test. GFLOPS/s\test. FLOPS/particle\n";

    // Iterate over all particle counts
    for(size_t i = 0; i < sizeof(updateBenchmarkParticleCounts) / sizeof(updateBenchmarkParticleCounts[0]); ++i)
    {
        ParticleSystem* pSys = createParticleSystem(updateBenchmarkParticleCounts[i]);

        Action* action = pSys->getActions()[0];
        action->preUpdateSignal.connect(benchmarkPreUpdateListener);
        action->postUpdateSignal.connect(benchmarkPostUpdateListener);

        // Reset times
        cpuSetUpTime = 0;
        gpuSetUpTime = 0;
        cpuUpdateTime = 0;
        gpuUpdateTime = 0;
        cpuCleanUpTime = 0;
        gpuCleanUpTime = 0;

        // Clear OpenGL queue
        glFinish();
        fpsClock.start();

        for(size_t r = 0; r < runsPerConfiguration; ++r)
        {
            gpuClock.start();
            cpuClock.start();

            // Set up time calculated in pre update listener

            engine->updateAllParticleSystems();

            // Update time calculated in post update listener

            cpuClock.stop();
            gpuClock.stop();
            glFinish();

            cpuCleanUpTime += cpuClock.getElapsedTime();
            gpuCleanUpTime += gpuClock.getElapsedTime();
        }

        fpsClock.stop();

        std::cout << std::to_string(updateBenchmarkParticleCounts[i]) << "\t"

                  << cpuSetUpTime / runsPerConfiguration << "\t"
                  << gpuSetUpTime / runsPerConfiguration << "\t"

                  << cpuUpdateTime / runsPerConfiguration << "\t"
                  << gpuUpdateTime / runsPerConfiguration << "\t"

                  << cpuCleanUpTime / runsPerConfiguration << "\t"
                  << gpuCleanUpTime / runsPerConfiguration << "\t"

                  << fpsClock.getElapsedTime() << "\t"
                  << 1.0 / ((fpsClock.getElapsedTime()) / runsPerConfiguration) << "\t"

                  << (getFlopsPerUpdate(updateBenchmarkParticleCounts[i]) * runsPerConfiguration) / (gpuUpdateTime * 1000000000) << "\t"
                  << getFlopsPerUpdate(updateBenchmarkParticleCounts[i]) / updateBenchmarkParticleCounts[i] << "\n";



        engine->deleteParticleSystem(pSys);
        pSys = nullptr;
    }

    engine->terminate();
    std::cout << "\nBenchmark completed successfully.\n";
}

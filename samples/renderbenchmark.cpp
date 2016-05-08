/*
 * Copyright (C) 2015 Simon Kerler
 *
 * This file is part of the "Nameless Particle Engine".
 * For conditions of distribution and use, see the copyright notice you should
 * have recievied with this software. I not, see:
 * http://opensource.org/licenses/Zlib
 */

/**
 * This application provides a utility to benchmark the render
 * performance of the engine.
 *
 * It subsequentially renders different amounts of particles. Particles
 * are represented by untessellated icosahedra. Each configuration is
 * rendered 1000 times to increase accuracy.
 *
 * Times for set up, render and clean up processes are profiled by using
 * the particle systems render events.
 **/

#include "engine.hpp"
#include "particlesystem.hpp"

#include "gpuclock.hpp"
#include "cpuclock.hpp"

using namespace nparticles;

const uint particleCounts[] =
{
    256,
    512,
    1024,
    2048,
    4096,
    8192,
    16384,
    32768,
    65536,
    131072,
    262144
};

// Clocks
GPUClock gpuClock;
CPUClock cpuClock;
CPUClock fpsClock;

// time variables
double cpuSetUpTime;
double gpuSetUpTime;
double cpuRenderTime;
double gpuRenderTime;
double cpuCleanUpTime;
double gpuCleanUpTime;

// Benchmark callbacks
void preRenderListener(ParticleSystem*, RenderSystem*)
{
    cpuClock.stop();
    gpuClock.stop();
    glFinish();

    cpuSetUpTime += cpuClock.getElapsedTime();
    gpuSetUpTime += gpuClock.getElapsedTime();

    gpuClock.start();
    cpuClock.start();
}

void postRenderListener(ParticleSystem*, RenderSystem*)
{
    cpuClock.stop();
    gpuClock.stop();
    glFinish();

    cpuRenderTime += cpuClock.getElapsedTime();
    gpuRenderTime += gpuClock.getElapsedTime();

    gpuClock.start();
    cpuClock.start();
}

int main(int argc, char* argv[])
{
    // Basic set up stuff
    Engine* engine = Engine::getInstance();
    engine->init(1440, 900, true, false);
    engine->useVSync(false);

    Camera& camera = engine->getCamera();
    camera.move(glm::vec3(0, 0, -2));

    GPUProgramService* gpuProgramService = engine->getGPUProgramService();
    gpuProgramService->addSourceDirectory("../res/shader/np", "/np");
    gpuProgramService->createRenderProgram("renderer", "/np/standard-vertex-shader.glsl", "/np/standard-fragment-shader.glsl");

    const Material* material = engine->getMaterialManager()->createMaterial("material", "renderer");
    const Mesh* mesh = engine->getMeshManager()->createIcosahedron("mesh");

    // The number of iterations performed by the benchmark loop
    uint runsPerConfiguration = 1000;

    // Print general information
    std::cout << "\n"
              << "+----------------------------+\n"
              << "| Rendering Benchmark        |\n"
              << "+----------------------------+\n"
              << "\n";

    std::cout << "Starting benchmark with resolution: 1440 x 900\nAll times are given in seconds.\n\n";

    std::cout << "# particles\t# vertices\tCPU set up\tGPU set up\tCPU render\tGPU render\tCPU clean up\tGPU clean up\tTotal time\tFPS\n";

    // Loop to benchmark rendering particle systems with different particle counts
    for(uint r = 0; r < sizeof(particleCounts) / sizeof(particleCounts[0]); ++r)
    {
        // Set up particle system with new particle count
        ParticleSystem* pSys = engine->createParticleSystem(particleCounts[r], *mesh, *material);
        auto positionsBuffer = pSys->addParticleAttribute<glm::vec4>("Positions");

        // Initialise particle positions
        glm::vec4* positionData = positionsBuffer->map();
        srand(time(nullptr));
        for(uint i = 0; i < particleCounts[r]; ++i)
            positionData[i] = glm::vec4(random() % 200 - 100, random() % 120 - 60, -100, 0);
        positionsBuffer->unmap();

        // Connect callbacks
        pSys->preRenderSignal.connect(preRenderListener);
        pSys->postRenderSignal.connect(postRenderListener);

        // Reset times
        cpuSetUpTime = 0;
        gpuSetUpTime = 0;
        cpuRenderTime = 0;
        gpuRenderTime = 0;
        cpuCleanUpTime = 0;
        gpuCleanUpTime = 0;

        // Clear GL queue
        glFinish();

        fpsClock.start();

        // Benchmark loop
        for(uint i = 0; i < runsPerConfiguration; ++i)
        {
            gpuClock.start();
            cpuClock.start();

            engine->drawAllParticleSystems();

            cpuClock.stop();
            gpuClock.stop();
            glFinish();

            cpuCleanUpTime += cpuClock.getElapsedTime();
            gpuCleanUpTime += gpuClock.getElapsedTime();
        }

        fpsClock.stop();

        // Print results of latest benchmark run
        std::cout << particleCounts[r] << "\t"
                  << particleCounts[r] * 12 << "\t"

                  << cpuSetUpTime / runsPerConfiguration << "\t"
                  << gpuSetUpTime / runsPerConfiguration << "\t"

                  << cpuRenderTime / runsPerConfiguration << "\t"
                  << gpuRenderTime / runsPerConfiguration << "\t"

                  << cpuCleanUpTime / runsPerConfiguration << "\t"
                  << gpuCleanUpTime / runsPerConfiguration << "\t"

                  << fpsClock.getElapsedTime() << "\t"
                  << 1.0 / (fpsClock.getElapsedTime() / runsPerConfiguration)
                  << std::endl;

        // Delete particle system
        engine->deleteParticleSystem(pSys);
    } // benchmark loop

    engine->terminate();
    std::cout << "\nBenchmark completed successfully.\n";
    return 0;
}

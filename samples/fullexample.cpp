/*
 * Copyright (C) 2015 Simon Kerler
 *
 * This file is part of the "Nameless Particle Engine".
 * For conditions of distribution and use, see the copyright notice you should
 * have recievied with this software. I not, see:
 * http://opensource.org/licenses/Zlib
 */

/**
 * This (little chaotic) sample application was used to
 * verify first implementations of the engine and is not
 * meant to be meaningful.
 *
 * It creates a particle system of 100 particles which are rendered
 * as triangles. To update shaders move them upwards and to the right
 * respectively.
 *
 * Several different aspects are covered by this application:
 * - Test the action list by executing two update shaders sequentially.
 * - Setting up custom uniforms and buffers for rendering.
 * - Create a custom mesh (a triangle).
 * - Pass a custom rotation matrix to the render process.
 */

#include <iostream>

#include "engine.hpp"
#include "meshmanager.hpp"
#include "materialmanager.hpp"
#include "particlesystem.hpp"
#include "cpuclock.hpp"
#include "gpuclock.hpp"

#include <glm/glm.hpp>
#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>

using namespace std;

// Struct used for testing buffers not based on a opengl type.
struct ParticleColorStruct
{
    glm::vec3 color;
    float alpha;
};

// Buffer storing uniform matrix.
nparticles::Buffer<glm::mat4>* uniformBuffer;

void psysPreRenderCallback(nparticles::ParticleSystem* psys, nparticles::RenderSystem* renderSystem)
{
    const nparticles::RenderProgram* renderProgram = renderSystem->getCurrentRenderProgram();
    renderProgram->setUniform("color", glm::vec3(0, 1, 0));

    auto in_buffer = dynamic_cast<nparticles::Buffer<ParticleColorStruct>*>(psys->getParticleAttributeBuffer("in_color"));
    renderSystem->setVertexAttribute("in_color", in_buffer, true);

    renderProgram->bindUniformBuffer("RotationMatrixBlock", uniformBuffer);
}

int main()
{
    // Set up engine
    nparticles::Engine* engine = nparticles::Engine::getInstance();
    engine->init(1680, 1050, false, true);

    nparticles::MaterialManager* materialManager = engine->getMaterialManager();
    nparticles::MeshManager* meshManager = engine->getMeshManager();
    nparticles::GPUProgramService* gpuProgramService = engine->getGPUProgramService();

    // Set up shaders
    gpuProgramService->addSourceDirectory("../res/shader", "/");
    gpuProgramService->addSourceDirectory("../res/shader/fullexample", "/fullexample");
    gpuProgramService->createRenderProgram("transform-renderer", "/fullexample/vertexshader.glsl", "/fullexample/fragmentshader.glsl");
    gpuProgramService->createComputeProgram("update-position-x", "/fullexample/update-position.glsl");
    gpuProgramService->createComputeProgram("update-position-y", "/fullexample/update-position-y.glsl");

    // Crete material
    const nparticles::Material* material = materialManager->createMaterial("transform-material", "transform-renderer");

    // Create a custom mesh
    glm::vec3 positions[3] =
    {
        {-1, 1, -0.5},
        {-1, 0.9, -0.5},
        {-0.9, 1, -0.5}
    };
    GLuint indices[] = {0, 1, 2};
    meshManager->createMesh("small-triangle", 3, positions, 3, indices);
    const nparticles::Mesh* mesh2 = meshManager->getMesh("small-triangle");

    // Set up particle system
    nparticles::ParticleSystem* pSys = engine->createParticleSystem(100,  *mesh2, *material);
    pSys->preRenderSignal.connect(psysPreRenderCallback);

    // Init b_position
    glm::vec4 b_positionData[100];
    for(int i = 0; i < 100; ++i)
        b_positionData[i] = glm::vec4(0.1 * (i % 10), -0.1 * (i / 10), 0, 0);

    auto b_positionBuffer = pSys->addParticleAttribute<glm::vec4>("b_position");
    b_positionBuffer->bindBase(GL_SHADER_STORAGE_BUFFER, 0);
    b_positionBuffer->setData(b_positionData);

    // Init custom vertex attribute
    ParticleColorStruct in_colorData[100];
    for(int i = 0; i < 100; ++i)
    {
        in_colorData[i].color = glm::vec3(0.01 * i, 0, 0);
        in_colorData[i].alpha = 1.0;
    }
    auto in_colorBuffer = pSys->addParticleAttribute<ParticleColorStruct>("in_color", GL_FLOAT, 4);
    in_colorBuffer->setData(in_colorData);

    pSys->appendAction(*gpuProgramService->getComputeProgram("update-position-x"));
    pSys->appendAction(*gpuProgramService->getComputeProgram("update-position-y"));

    // Custom uniform buffer
    glm::mat4 uniformBufferData(1);
    uniformBuffer = new nparticles::Buffer<glm::mat4>(1, GL_FLOAT, -1, GL_STREAM_DRAW, GL_UNIFORM_BUFFER);
    uniformBuffer->setData(&uniformBufferData);

    // Main loop
    while(!engine->windowClosed())
    {
        engine->processEvents();

        uniformBufferData = glm::rotate(uniformBufferData, 0.001f, glm::vec3(0, 0, 1));
        uniformBuffer->setData(&uniformBufferData);

        engine->updateAllParticleSystems();
        engine->drawAllParticleSystems();
    }

    delete uniformBuffer;
    engine->terminate();

    return 0;
}


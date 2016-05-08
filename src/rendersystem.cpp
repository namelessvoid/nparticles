/*
 * Copyright (C) 2015 Simon Kerler
 *
 * This file is part of the "Nameless Particle Engine".
 * For conditions of distribution and use, see the copyright notice you should
 * have recievied with this software. I not, see:
 * http://opensource.org/licenses/Zlib
 */

#include "rendersystem.hpp"

#include "logger.hpp"
#include "mesh.hpp"
#include "material.hpp"
#include "particlesystem.hpp"

#include "glutils.hpp"

namespace nparticles
{
void RenderSystem::setViewProjectionMatrix(const glm::mat4& viewProjectionMatrix)
{
    mViewProjectionMatrix = viewProjectionMatrix;
}

void RenderSystem::setNormalMatrix(const glm::mat3& normalMatrix)
{
    mNormalMatrix = normalMatrix;
}

RenderSystem::RenderSystem()
{
}

RenderSystem::~RenderSystem()
{
    terminate();
}

GLFWwindow* RenderSystem::init(int width, int height, bool fullscreen, bool debug)
{
    // Begin initialisation
    if(!glfwInit())
    {
        Logger::getInstance()->logError("RenderSystem: Failed to initialise GLFW!");
        return nullptr;
    }

    Logger::getInstance()->logInfo("RenderSystem: GLFW initialised.");

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_FALSE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create debug OpenGL context
    if(debug)
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

    GLFWmonitor* monitor = fullscreen?glfwGetPrimaryMonitor():nullptr;
    mWindow = glfwCreateWindow(width, height, "Particle Engine by namelessvoid", monitor, NULL);

    if(!mWindow)
    {
        Logger::getInstance()->logError("RenderSystem: Could not create GLFW window!");
        terminate();
        return nullptr;
    }

    Logger::getInstance()->logInfo("RenderSystem: GLFW Window created.");

    glfwMakeContextCurrent(mWindow);

    glewExperimental = GL_TRUE;
    glewInit();

    Logger::getInstance()->logInfo("RenderSystem: GLEW initialised.");

    // Register the debug message callback
    if(debug)
    {
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(glutils::glDebugCallback, nullptr);
        glutils::setGlDebugLevel(GL_DEBUG_SEVERITY_LOW);
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_PROGRAM_POINT_SIZE);

    return mWindow;
}

void RenderSystem::terminate()
{
    glfwSetWindowShouldClose(mWindow, true);
    glfwTerminate();
    Logger::getInstance()->logInfo("RenderSystem: terminated.");
}

void RenderSystem::beginFrame()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RenderSystem::endFrame()
{
    glfwSwapBuffers(mWindow);
}

void RenderSystem::useVSync(bool vSync)
{
    // enable vsync
    if(vSync)
        glfwSwapInterval(1);
    // disable vsync
    else
        glfwSwapInterval(0);
}

void RenderSystem::useDepthTest(bool depthTest)
{
    if(depthTest)
        glEnable(GL_DEPTH_TEST);
    else
        glDisable(GL_DEPTH_TEST);
}

void RenderSystem::drawParticleSystem(ParticleSystem* particleSystem)
{
    const Mesh* mesh = particleSystem->getMesh();
    const Material* material = particleSystem->getMaterial();

    Buffer<glm::vec3>* vertexBuffer = ((Mesh*)mesh)->getVertexBuffer();
    Buffer<GLuint>* indexBuffer = ((Mesh*)mesh)->getIndexBuffer();

    // Bind and set up material
    mCurrentRenderProgram = material->getRenderProgram();
    mCurrentRenderProgram->bind();
    mCurrentRenderProgram->setUniform("np_viewProjectionMatrix", mViewProjectionMatrix);
    mCurrentRenderProgram->setUniform("np_normalMatrix", mNormalMatrix);

    // Bind and setup geometry
    mesh->bind();
    setVertexAttribute("np_in_position", vertexBuffer);
    indexBuffer->bind(GL_ELEMENT_ARRAY_BUFFER);

    // Bind particle attributes, atomic counters and uniform buffers
    bindParticleBuffers(particleSystem, mCurrentRenderProgram);

    // Emit pre render signal
    particleSystem->emitPreRenderSignal(this);

    // Activate subroutines. Dot his after the preRenderSignal so user selected subroutines are activated.
    mCurrentRenderProgram->activateSubroutines();

    // Draw everything
    GLenum renderType = GL_TRIANGLES;

    // Set up tessellation
    if(mCurrentRenderProgram->usesTessellation())
    {
        renderType = GL_PATCHES;
        glPatchParameteri(GL_PATCH_VERTICES, mCurrentRenderProgram->getTessellationPatchSize());
    }
    else if(material->getRenderType() == NP_RT_POINTS)
        renderType = GL_POINTS;

    glDrawElementsInstanced(renderType, indexBuffer->getItemCount(), indexBuffer->getGlType(), nullptr, particleSystem->getParticleCount());

    // emit post render signal
    particleSystem->emitPostRenderSignal(this);

    // Unbind all resources
    // Attribute pointers
    for(auto vertexAttributeBinding : mVertexAttributeBindings)
        glDisableVertexAttribArray(vertexAttributeBinding);
    mVertexAttributeBindings.clear();

    // Unbind particle attributes, atomic counters and uniform buffers
    unbindParticleBuffers(particleSystem);

    // Mesh
    indexBuffer->unbind();
    mesh->unbind();

    // Material
    mCurrentRenderProgram->unbind();
    mCurrentRenderProgram = nullptr;
}

} // namespace nparticles

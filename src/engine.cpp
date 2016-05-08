/*
 * Copyright (C) 2015 Simon Kerler
 *
 * This file is part of the "Nameless Particle Engine".
 * For conditions of distribution and use, see the copyright notice you should
 * have recievied with this software. I not, see:
 * http://opensource.org/licenses/Zlib
 */

#include "engine.hpp"

#include "logger.hpp"
#include "particlesystem.hpp"

namespace nparticles
{

Engine::Engine()
    : mRenderSystem(),
      mWindow(nullptr),
      mGPUProgramService()
{
}


Engine::~Engine()
{
    for(auto pSys : mParticleSystems)
        delete pSys;
    mParticleSystems.clear();
}

void Engine::staticKeyCallback(GLFWwindow* window, int key, int, int action, int modifier)
{
    Engine::getInstance()->keyCallback(window, key, action, modifier);
}

void Engine::keyCallback(GLFWwindow* window, int key, int action, int modifier)
{
    // Only process events belonging to this window. Boilerplate :)
    if(window != mWindow)
        return;

    // Reset movement when key was released.
    float speed = 0.1;

    if(action == GLFW_RELEASE)
        speed = 0;
    else if(modifier & GLFW_MOD_SHIFT)
        speed *= 100;
    else if(modifier & GLFW_MOD_CONTROL)
        speed *= 0.01;

    switch(key)
    {
    case GLFW_KEY_W:
        mCamera.setMovementSpeed(speed, Camera::NP_DIR_FOREWARD);
        break;
    case GLFW_KEY_S:
        mCamera.setMovementSpeed(speed, Camera::NP_DIR_BACKWARD);
        break;
    case GLFW_KEY_A:
        mCamera.setMovementSpeed(speed, Camera::NP_DIR_STRAFE_LEFT);
        break;
    case GLFW_KEY_D:
        mCamera.setMovementSpeed(speed, Camera::NP_DIR_STRAFE_RIGHT);
        break;
    case GLFW_KEY_E:
        mCamera.setMovementSpeed(speed, Camera::NP_DIR_UP);
        break;
    case GLFW_KEY_Q:
        mCamera.setMovementSpeed(speed, Camera::NP_DIR_DOWN);
        break;
    case GLFW_KEY_Y: // fall through
    case GLFW_KEY_Z:
        mCamera.rotate(-0.1, Camera::NP_ROT_ROLL);
        break;
    case GLFW_KEY_X:
        mCamera.rotate(0.1, Camera::NP_ROT_ROLL);
        break;
    // All others are ignored
    default:
        break;
    }

    keyEventSignal.emit(key, action, modifier);
}

void Engine::staticCurserPosCallback(GLFWwindow* window, double xpos, double ypos)
{
    Engine::getInstance()->cursorPosCallback(window, xpos, ypos);
}

void Engine::cursorPosCallback(GLFWwindow* window, double xpos, double ypos)
{
    if(glfwGetMouseButton(mWindow, GLFW_MOUSE_BUTTON_1) != GLFW_PRESS)
        return;

    double dx = xpos - mLastCursorPosition.x;
    double dy = ypos - mLastCursorPosition.y;

    mCamera.rotate(-0.01 * dx, Camera::NP_ROT_YAW);
    mCamera.rotate(-0.01 * dy, Camera::NP_ROT_PITCH);

    mLastCursorPosition = glm::dvec2(xpos, ypos);
}

void Engine::staticMouseButtonCallback(GLFWwindow* window, int button, int action, int modifier)
{
    Engine::getInstance()->mouseButtonCallback(window, button, action, modifier);
}

void Engine::mouseButtonCallback(GLFWwindow* window, int button, int action, int)
{
    if(button == GLFW_MOUSE_BUTTON_1 && action == GLFW_PRESS)
        glfwGetCursorPos(window, &mLastCursorPosition.x, &mLastCursorPosition.y);
}

void Engine::init(int width, int height, bool fullscreen, bool debug)
{
    Logger::getInstance()->disable(!debug);
    mWindow = mRenderSystem.init(width, height, fullscreen, debug);

    // Ensure GL_ARB_shading_language_include is available.
    if(!GLEW_ARB_shading_language_include)
    {
        Logger::getInstance()->logError("Engine initialisation failed: GL_ARB_shading_language_include extension required but not available!");
        exit(-1);
    }

    // Init camera
    mCamera.setPerspective(M_PI / 2.5, width, height, 0.1, 5000000);
    glfwGetCursorPos(mWindow, &mLastCursorPosition.x, &mLastCursorPosition.y);

    // Init input methods
    glfwSetKeyCallback(mWindow, staticKeyCallback);
    glfwSetCursorPosCallback(mWindow, staticCurserPosCallback);
    glfwSetMouseButtonCallback(mWindow, staticMouseButtonCallback);

}

void Engine::terminate()
{
    mRenderSystem.terminate();
}

void Engine::updateAllParticleSystems()
{
    for(auto pSys : mParticleSystems)
        mComputeSystem.updateParticleSystem(pSys);
}

void Engine::drawAllParticleSystems()
{
    mCamera.updatePosition();

    mRenderSystem.beginFrame();
    mRenderSystem.setViewProjectionMatrix(mCamera.getViewProjectionMatrix());
    mRenderSystem.setNormalMatrix(mCamera.getNormalMatrix());

    // iterate all particle systems and call renderSystem.drawParticleSystem(psys);
    for(auto pSys : mParticleSystems)
        mRenderSystem.drawParticleSystem(pSys);

    mRenderSystem.endFrame();
}

void Engine::processEvents()
{
    glfwPollEvents();
}

bool Engine::windowClosed()
{
    return glfwWindowShouldClose(mWindow);
}

ParticleSystem* Engine::createParticleSystem(int particleCount, const Mesh& mesh, const Material& material)
{
    ParticleSystem* pSys = new ParticleSystem(particleCount, mesh, material);
    mParticleSystems.insert(pSys);
    return pSys;
}

void Engine::deleteParticleSystem(ParticleSystem* particleSystem)
{
    auto pSysIter = mParticleSystems.find(particleSystem);
    if(pSysIter != mParticleSystems.end())
    {
        delete particleSystem;
        mParticleSystems.erase(pSysIter);
    }
}

} // namespace nparticles

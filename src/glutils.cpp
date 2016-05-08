/*
 * Copyright (C) 2015 Simon Kerler
 *
 * This file is part of the "Nameless Particle Engine".
 * For conditions of distribution and use, see the copyright notice you should
 * have recievied with this software. I not, see:
 * http://opensource.org/licenses/Zlib
 */

#include "glutils.hpp"

#include "logger.hpp"

namespace nparticles
{
namespace glutils
{

GLint glGet(GLenum target, int index)
{
    GLint value = 0;

    // If called with a buffer binding point, translate it to the corresponding _BINDING name.
    if(target == GL_ARRAY_BUFFER)
        target = GL_ARRAY_BUFFER_BINDING;
    else if(target == GL_ELEMENT_ARRAY_BUFFER)
        target = GL_ELEMENT_ARRAY_BUFFER_BINDING;
    else if(target == GL_SHADER_STORAGE_BUFFER)
        target = GL_SHADER_STORAGE_BUFFER_BINDING;
    else if(target == GL_UNIFORM_BUFFER)
        target = GL_UNIFORM_BUFFER_BINDING;
    else if(target == GL_ATOMIC_COUNTER_BUFFER)
        target = GL_ATOMIC_COUNTER_BUFFER_BINDING;

    if(index > 0)
        glGetIntegeri_v(target, index, &value);
    else
        glGetIntegerv(target, &value);

    return value;
}

std::string shaderTypeToString(GLenum shaderType)
{
    switch(shaderType)
    {
    case GL_VERTEX_SHADER:          return "GL_VERTEX_SHADER";
    case GL_FRAGMENT_SHADER:        return "GL_FRAGMENT_SHADER";
    case GL_TESS_CONTROL_SHADER:    return "GL_TESS_CONTROL_SHADER";
    case GL_TESS_EVALUATION_SHADER: return "GL_TESS_EVALUATION_SHADER";
    case GL_GEOMETRY_SHADER:        return "GL_GEOMETRY_SHADER";
    case GL_COMPUTE_SHADER:         return "GL_COMPUTE_SHADER";
    default: return "invalid shader type";
    }
}

void setGlDebugLevel(GLenum severity)
{
    // First, disable all severity levels.
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, false);

    // Falltrough switch, to enable severity level and all higher levels.
    switch(severity)
    {
    case GL_DEBUG_SEVERITY_NOTIFICATION:
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, true);
        // fallthrough
    case GL_DEBUG_SEVERITY_LOW:
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_LOW, 0, nullptr, true);
        // fallthrough
    case GL_DEBUG_SEVERITY_MEDIUM:
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_MEDIUM, 0, nullptr, true);
        // fallthrough
    case GL_DEBUG_SEVERITY_HIGH:
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_HIGH, 0, nullptr, true);
        break;
    }

}

void glDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
    Logger* logger = Logger::getInstance();

    std::string m = std::string(message) + "(" + std::to_string(id) + ")";

    switch(severity)
    {
    case GL_DEBUG_SEVERITY_NOTIFICATION:
        logger->logInfo(m);
        break;
    case GL_DEBUG_SEVERITY_LOW: // fallthrough
    case GL_DEBUG_SEVERITY_MEDIUM:
        logger->logWarning(m);
        break;
    case GL_DEBUG_SEVERITY_HIGH:
        logger->logError(m);
        break;
    }
}


} // namespace glutils
} // namespace nparticles

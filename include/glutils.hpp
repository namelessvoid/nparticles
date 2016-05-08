/*
 * Copyright (C) 2015 Simon Kerler
 *
 * This file is part of the "Nameless Particle Engine".
 * For conditions of distribution and use, see the copyright notice you should
 * have recievied with this software. I not, see:
 * http://opensource.org/licenses/Zlib
 */

#ifndef NP_GLUTILS_HPP
#define NP_GLUTILS_HPP

#include <GL/glew.h>

#include <typeindex>
#include <string>

#include <glm/glm.hpp>

namespace nparticles
{

/**
 * The glutils namspace contains generic OpenGL helper methods and classes.
 */
namespace glutils
{

/**
 * The GlTypeInfo class stores how a C++ type is seen by OpenGL.
 *
 * This is useful to determine the GL type and size when calling (size and type):
 *
 * @code
 * void glVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid * pointer);
 * @endcode
 *
 * Examples:
 *
 * @code
 * C++ type    GL type   base size
 * float       GL_FLOAT  1
 * glm::ivec4  GL_INT    4
 * @endcode
 */
class GlTypeInfo
{
public:
    /**
     * The GlType constructor.
     *
     * @param glType The OpenGL type.
     * @param baseSize The size of the type.
     */
    GlTypeInfo(GLenum glType, int baseSize)
        : mGlType(glType),
          mBaseSize(baseSize)
    {}

    /**
     * Equals operator.
     *
     * @param rhs The GlTypeInfo to compare to.
     *
     * @return True if both GlTypeInfos are equal, false otherwise.
     */
    bool operator==(const GlTypeInfo& rhs) const
    {
        return (this->mBaseSize == rhs.mBaseSize) && (this->mGlType == rhs.mGlType);
    }

    /**
     * The OpenGL type.
     *
     * E.g. GL_FLOAT for float.
     */
    GLenum mGlType;

    /**
     * The base size of the OpenGL type.
     *
     * E.g. 4 for glm::vec4.
     */
    int mBaseSize;
};

/**
 * Get the GlTypeInfo for a given variable.
 *
 * This returns the GlTypeInfo for a known variable.
 *
 * @note Supported variables are
 * - booleans:          bool, glm::bvec2, glm::bvec3, glm::bvec4
 * - integers:          int,  glm::ivec2, glm::ivec3, glm::ivec4
 * - unsigned integers: unsigned int
 * - floats:            float, glm::vec2, glm::vec3, glm::vec4
 *
 * @param variable The variable to guess GlTypeInfo from.
 * @tparam The type of @p variable.
 *
 * @return A GlTypeInfo object containing the type info for given @p variable. If @p variable is not
 *         of one of the supported types, GlTypeInfo(GL_INVALID_VALUE, -1) is returned.
 */
template<typename T>
GlTypeInfo getGlTypeInfo(const T& variable)
{
    std::type_index type = typeid(variable);

    // booleans
    if( type == typeid(bool) ||
        type == typeid(glm::bvec2) ||
        type == typeid(glm::bvec3) ||
        type == typeid(glm::bvec4)
       ) return {GL_BOOL, sizeof(variable) / sizeof(bool)};

    // integers
    if( type == typeid(int) ||
        type == typeid(glm::ivec2) ||
        type == typeid(glm::ivec3) ||
        type == typeid(glm::ivec4)
      ) return {GL_INT, sizeof(variable) / sizeof(int)};

    // unsigned integers
    if( type == typeid(unsigned int)
      ) return {GL_UNSIGNED_INT, 1};


    // floats
    if( type == typeid(float) ||
        type == typeid(glm::vec2) ||
        type == typeid(glm::vec3) ||
        type == typeid(glm::vec4)
      ) return {GL_FLOAT, sizeof(variable) / sizeof(float)};

    // nothing matched, return GL_INVALID_VALUE since this
    // type is not supported by nparticles.
    return {GL_INVALID_VALUE, -1};
}

/**
 * Wrapper for glGetInteger*() functions.
 *
 * This method wraps the glGetIntegerv() and glGetIntegeri_v() functions.
 *
 * @param target The queried target (see glGet documentation).
 * @param index The index to query (used with glGetIntegeri_v()). Defaults to 0.
 *
 * @return The queried value.
 */
GLint glGet(GLenum target, int index = 0);

/**
 * Convert the numeric type of a shader to string.
 *
 * @param shaderType One of the OpenGL shader types.
 *
 * @return Name of the shader or "invalid shader type" if shaderType is none of the OpenGL shader types.
 */
std::string shaderTypeToString(GLenum shaderType);

/**
 * Set the debug level used by glDebugCallback().
 *
 * This sets the debug level to @p severity. All debug messages with equal or higher severity are logged.
 *
 * @param severity The OpenGL severity level e.g. GL_DEBUG_SEVERITY_MEDIUM.
 */
void setGlDebugLevel(GLenum severity);

/**
 * Callback method for OpenGL debug messages.
 *
 * When OpenGL is run in debug context, debug messages are created and passed to this function.
 *
 * @param source Sender of the message.
 * @param type The type of the message.
 * @param id Id of the message.
 * @param severity Severity level of the message.
 * @param length Lenght of the @p message string.
 * @param message String with a human readable debug message.
 * @param userParam User defined parameters.
 */
void glDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void* userParam );

} // namespace glutils
} // namespace nparticles

#endif // NP_GLUTILS_HPP

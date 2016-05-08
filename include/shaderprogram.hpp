/*
 * Copyright (C) 2015 Simon Kerler
 *
 * This file is part of the "Nameless Particle Engine".
 * For conditions of distribution and use, see the copyright notice you should
 * have recievied with this software. I not, see:
 * http://opensource.org/licenses/Zlib
 */

#ifndef NP_SHADERPROGRAM_HPP
#define NP_SHADERPROGRAM_HPP

#include <GL/glew.h>

#include <string>
#include <map>

#include <glm/glm.hpp>

#include "logger.hpp"
#include "buffer.hpp"

namespace nparticles
{

/**
 * The ShaderProgram class encapsulates OpenGL shader programs.
 *
 * This class encapsulates OpenGL shader programs. It serves as a base class for RenderProgram and
 * ComputeProgram classes.
 *
 * @note This class must not be instantiated directly! To create a new RenderProgram or ComputeProgram use the
 * GPUProgramService which can be obtained by the Engine via Engine::getGPUProgramService!
 */
class ShaderProgram
{
public:
    /**
     * Bind the ShaderProgram.
     *
     * Activates the ShaderProgram by binding it to the current OpenGL context. If the program was not
     * built successfully, false is returned to indicate, that it cannot be bound.
     *
     * @return False, if the ShaderProgram was not successfully built or if ShaderProgram::build() was not
     *         called.
     */
    bool bind() const;

    /**
     * Unbind the ShaderProgram.
     *
     * This unbinds the ShaderProgram previously bound with ShaderProgram::bind().
     *
     * @note Actually this call unbinds *any* ShaderProgram currently bound, so this method should be used with care!
     */
    void unbind() const;

    /**
     * Build the ShaderProgram.
     *
     * This method compiles all attached shader objects and links the OpenGL program. Errors occured during build are
     * logged by the Logger class.
     *
     * @return False, if build failed. True if build was successful.
     */
    bool build();

    /**
     * Return the index of a uniform variable.
     *
     * This queries the location of a uniform variable named @p name. The index of the uniform variable is returned
     * or -1 if no such uniform exists or the ShaderProgram was not successfully built.
     *
     * If you have declared a uniform but cannot retrieve its location, it maybe fell a victim to the drivers compiler
     * optimisations.
     *
     * @param name String containing the name of the queried uniform variable.
     *
     * @return The location of the uniform variable or -1 if either no such variable exists or the program was not
     *         successfully built via ShaderProgram::build().
     */
    GLint getUniformLocation(const std::string& name) const;

    /**
     * Set uniform variable of type bool.
     *
     * This is one of the overlaoded setUniform() methods responsible for setting
     * boolean variables.
     *
     * @param name String containing the name of a uniform variable.
     * @param value The value to set to the specified uniform variable.
     *
     * @return True if variable @p name exists, otherwise false.
     */
    bool setUniform(const std::string& name, bool value) const;

    /**
     * Set uniform variable of type float.
     *
     * This is one of the overlaoded setUniform() methods responsible for setting
     * float variables.
     *
     * @param name String containing the name of a uniform variable.
     * @param value The value to set to the specified uniform variable.
     *
     * @return True if variable @p name exists, otherwise false.
     */
    bool setUniform(const std::string& name, float value) const;

    /**
     * Set a uniform variable of type mat4.
     *
     * This is one of the overlaoded setUniform() methods responsible for setting
     * 4x4 float matrices.
     *
     * @param name String containing the name of a uniform variable.
     * @param value The value to set to the specified uniform variable.
     * @param transpose Whether the matrix should be transposed or not. Defaultes to false.
     *
     * @return True if variable @p name exists, otherwise false.
     */
    bool setUniform(const std::string& name, const glm::mat4& value, bool transpose = false) const;

    /**
     * Set a uniform variable of type mat3.
     *
     * This is one of the overloaded setUniform() methods responsible for setting 3x3
     * float matrices.
     *
     * @param name String containing the name of a uniform variable.
     * @param value The value to set to the specified uniform matrix.
     * @param transpose Whether the matrix should be transposed or not. Defaults to false.
     *
     * @return True if variable @p name exists, or false otherwise.
     */
    bool setUniform(const std::string& name, const glm::mat3& value, bool transpose = false) const;

    /**
     * Set a uniform variable of type vec3.
     *
     * This is one of the overloaded setUniform() methods responsible for setting
     * 3-dimensional float vectors.
     *
     * @param name String containing the name of a uniform variable.
     * @param value The value to set to the specified uniform variable.
     *
     * @return True if variable @p name exists, otherwise false.
     */
    bool setUniform(const std::string& name, const glm::vec3& value) const;

    /**
     * Set a uniform variable of type uint.
     *
     * This is one of the overlaoded setUniform() methods responsible for setting a
     * unsigned integer variable.
     *
     * @param name String containing the name of a uniform variable.
     * @param value The value to set to the specified uniform variable.
     *
     * @return True if variable @p name exists, or false otherwise.
     */
    bool setUniform(const std::string& name, uint value) const;

    /**
     * Bind a Buffer to a shader storage block.
     *
     * Bind the Buffer @p buffer to a shader storage block named @p storageBlockName. To do so, the Buffer
     * is bound to GL_SHADER_STORAGE_BUFFER, reverting all previous bindings of the Buffer (see Buffer::bindBase()).
     *
     * @param storageBlockName String containing the name of the shader storage block inside the shader program to which the
     *                         Buffer should be bound.
     * @param buffer Pointer to a BaseBuffer object to be bound to the given shader storage block.
     *
     * @return True, if @p buffer was successfully bound, i.e. @p buffer is bound to target GL_SHADER_STORAGE_BUFFER and
     *         there is a shader storage block named @p storageBlockName within the ShaderProgram.
     */
    bool bindShaderStorageBuffer(const std::string& storageBlockName, BufferBase* buffer) const;

    /**
     * Bind a Buffer to a uniform block.
     *
     * Bind the Buffer @p buffer to a uniform block named @p uniformBlockName. To do so, the Buffer
     * is bound to GL_UNIFORM_BUFFER, reverting all previous bindings of the Buffer (see BufferBase::bindBase()).
     *
     * @param uniformBlockName String containing the name of the uniform block inside the shader program to which the
     *                         Buffer should be bound.
     * @param buffer Pointer to a BaseBuffer object to be bound to the given uniform block.
     *
     * @return True, if @p buffer was successfully bound, i.e. there is a uniform block named @p uniformBlockName within the ShaderProgram.
     */
    bool bindUniformBuffer(const std::string& uniformBlockName, BufferBase* buffer) const;

    /**
     * Bind a Buffer as an atomic counter.
     *
     * Bind the Buffer @p buffer to an atomic counter named @p atomicCounterName. To do so, the Buffer is
     * bound to GL_ATOMIC_COUNTER_BUFFER, reverting all previous bindings of the Buffer (see Buffer::bindBase()).
     *
     * @param atomicCounterName String containing the name of the atomic counter uniform variable inside the shader program.
     *                          The Buffer is bound to this variable.
     * @param buffer Pointer to a BaseBuffer object to be bound to the given atomic counter variable.
     *
     * @return True, if @p buffer was successfully bound, i.e. there is an atomic counter named @p atomicCounterName within the ShaderProgram.
     */
    bool bindAtomicCounterBuffer(const std::string& atomicCounterName, BufferBase* buffer) const;

    /**
     * Enables all selected subroutines.
     *
     * This method has to be called to activate the subroutines selected with selectSubroutines().
     *
     * @note This is called by the RenderSystem *after* the preUpdateSignal is emitted, so subroutines selected by callbacks to this signal
     *       are activated properly. There is no need to call this method manually!
     */
    void activateSubroutines() const;

    /**
     * Select a subroutine.
     *
     * This selects the subroutine with name @p subroutineName and assigns it to the selector ("subroutine uniform") named
     * @p selectorName. This simplifies the process of using subruotines in GLSL shaders.
     *
     * @param shaderStage The stage of which the subroutine is selected. This is one of the OpenGL stage enums (GL_VERTEX_SHADER etc.).
     * @param selectorName The name of the selector ("subroutine uniform") specified in the shader source.
     * @param subroutineName A name of a subroutine that can be bound to the selector.
     *
     * @return True if @p selectorName and @p subroutineName are valid selector / routine names for given shader stage.
     */
    bool selectSubroutine(GLenum shaderStage, std::string selectorName, std::string subroutineName);

protected:
    /**
     * ShaderProgram construtor.
     */
    ShaderProgram();

    /**
     * Virtual ShaderProgram destructor.
     */
    virtual ~ShaderProgram();

    /**
     * Add a new shader to the program.
     *
     * This creates a new shader of @type with source code @sourceCode and attaches it to the program.
     * The shader is not compiled until ShaderProgram::build() is called.
     *
     * This is only possible if the program was not already built successfully.
     *
     * Any shader of the same @p type previously added to the program is overwritten!
     *
     * @param type The type of the shader which must be one of the valid OpenGL shader types.
     * @param sourceCode String containing the source code used for this shader.
     *
     * @return True, if shader was successfully added. False, if program was already built or @p sourceCode is
     *         an empty string.
     */
    bool addNewShader(GLenum type, std::string sourceCode);

    /**
     * The OpenGL shader program handle.
     *
     * The OpenGL shader program handle returned by glCreateProgram().
     */
    GLuint mShaderProgram;

private:
    /**
     * Compile a shader.
     *
     * This method is used to compile a single shader.
     *
     * This method should never be called manually. Use ShaderProgram::build() instead.
     *
     * @param shader The OpenGL shader handle.
     * @param type The OpenGL shader type.
     *
     * @return True, if shader compiled successfully, false otherwise.
     */
    bool compileShader(GLuint shader, GLenum type);

    /**
     * Link the ShaderProgram.
     *
     * This method links the ShaderProgram.
     *
     * This methos should never be called manually. Use ShaderProgram::build()
     * instead.
     *
     * @return True, if ShaderProgram was linked successfully, false otherwise.
     */
    bool link();

    /**
     * Bind a Buffer to a shader variable.
     *
     * This method is used to bind a buffer to a shader variable defined in the shader soruce. In this process, the buffer is bound to
     * @p bufferTarget which also determines the type of the variable that is bound.
     *
     * The variable @p shaderVariableName is queried from the ShaderProgram to get the binding index on which the @p buffer is bound to
     * be accessible in the shader.
     *
     * @param bufferTarget The target of the buffer. Must be GL_UNIFORM_BUFFER, GL_SHADER_STORAGE_BUFFER or GL_ATOMIC_COUNTER_BUFFER.
     * @param shaderVariableName The name of the variable as specified in the shader source code.
     * @param buffer The Buffer to be bound to the variable.
     *
     * @return True, if binding was successful. False, if no block @p blockName could be found for given @p bufferTarget.
     */
    bool bindBufferToShaderVariable(GLenum bufferTarget, const std::string& shaderVariableName, BufferBase* buffer) const;

    /**
     * Retrieve subroutine information from a shader.
     *
     * Query information about subroutines and initialises mSubroutines with failsave defaults. The number of subroutine
     * uniforms for given stage are saved in mSubroutineCount.
     *
     * This is queried directly after a successful build().
     *
     * @param shaderStage The shader stage is one of the valid OpenGL shader names like GL_FRAGMENT_SHADER.
     */
    void querySubroutines(GLenum shaderStage);

    /**
     * The build status of the ShaderProgram.
     *
     * True, if ShaderProgram was built successfully, otherwise false.
     */
    bool mBuildStatus;

    /**
     * Map containing all added shaders.
     *
     * Key is the OpenGL shader type, value the shader handle returned by glCreateShader().
     */
    std::map<GLenum, GLuint> mShaderMap;

    /**
     * Stores the currently selected subroutines for all subroutine uniforms.
     *
     * Key: The shader stage to which the subroutine uniforms belong.
     * Value: Array storing the size of active subroutine uniforms in the shader stage. The content of the array
     *        is the index of a active subroutine function which is associated with this subroutine uniform.
     */
    std::map<GLenum, GLuint*> mActiveSubroutines;

    /**
     * The number of active subroutine uniforms per shader stage.
     *
     * This map stores for all shader stages (key) the number of active subroutine uniforms (value). This equals to the
     * size of the array stored in mActiveSubroutines for the corresponding stage.
     */
    std::map<GLenum, GLint> mSubroutineUniformCount;

    // Hide copy constructor and assignment operator
    ShaderProgram(const ShaderProgram&) = delete;
    void operator=(const ShaderProgram&) = delete;
};

} // namespace nparticles
#endif // NP_SHADERPROGRAM_HPP

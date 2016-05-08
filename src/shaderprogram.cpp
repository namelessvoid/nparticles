/*
 * Copyright (C) 2015 Simon Kerler
 *
 * This file is part of the "Nameless Particle Engine".
 * For conditions of distribution and use, see the copyright notice you should
 * have recievied with this software. I not, see:
 * http://opensource.org/licenses/Zlib
 */

#include "shaderprogram.hpp"

namespace nparticles
{

bool ShaderProgram::bind() const
{
    if(!mBuildStatus)
    {
        Logger::getInstance()->logWarning("Attemp to bind unbuilt ShaderProgram ignored.");
        return false;
    }

    glUseProgram(mShaderProgram);
    return true;
}

void ShaderProgram::unbind() const
{
    glUseProgram(0);
}

bool ShaderProgram::build()
{
    // Return immediately, if program was already built.
    // Attention: this makes it impossible to relink the program! Maybe remove if this becomes necessary.
    if(mBuildStatus)
    {
        Logger::getInstance()->logWarning("Attemp of rebuilding already built shader ignored.");
        return false;
    }

    for(auto shaderIter : mShaderMap)
    {
        if(!compileShader(shaderIter.second, shaderIter.first))
            return false;
    }

    mBuildStatus = link();

    // Query subroitines if build was successful.
    if(mBuildStatus)
        for(auto shaderIter : mShaderMap)
            querySubroutines(shaderIter.first);

    return mBuildStatus;
}

GLint ShaderProgram::getUniformLocation(const std::string& name) const
{
    if(!mBuildStatus)
        return -1;
    return glGetUniformLocation(mShaderProgram, name.c_str());
}

bool ShaderProgram::setUniform(const std::string& name, bool value) const
{
    return setUniform(name, (uint)value);
}

bool ShaderProgram::setUniform(const std::string& name, float value) const
{
    GLint location = getUniformLocation(name);

    if(location == -1)
        return false;

    glProgramUniform1f(mShaderProgram, location, value);
    return true;
}

bool ShaderProgram::setUniform(const std::string& name, const glm::mat4& value, bool transpose) const
{
    GLint location = getUniformLocation(name);

    if(location == -1)
        return false;

    glProgramUniformMatrix4fv(mShaderProgram, location, 1, transpose, &value[0][0]);
    return true;
}

bool ShaderProgram::setUniform(const std::string& name, const glm::mat3& value, bool transpose) const
{
    GLint location = getUniformLocation(name);

    if(location == -1)
        return false;

    glProgramUniformMatrix3fv(mShaderProgram, location, 1, transpose, &value[0][0]);
    return true;
}

bool ShaderProgram::setUniform(const std::string& name, const glm::vec3& value) const
{
    GLint location = getUniformLocation(name);

    if(location == -1)
        return false;

    glProgramUniform3fv(mShaderProgram, location, 1, &value[0]);
    return true;
}

bool ShaderProgram::setUniform(const std::string& name, uint value) const
{
    GLint location = getUniformLocation(name);

    if(location == -1)
        return false;

    glProgramUniform1ui(mShaderProgram, location, value);
    return true;
}

ShaderProgram::ShaderProgram()
    : mBuildStatus(false)
{
    mShaderProgram = glCreateProgram();
}

ShaderProgram::~ShaderProgram()
{
    for(auto shaderIter : mShaderMap)
        glDeleteShader(shaderIter.second);
    glDeleteProgram(mShaderProgram);

    for(auto subroutineIter : mActiveSubroutines)
        delete[] subroutineIter.second;
    mActiveSubroutines.clear();
}

bool ShaderProgram::addNewShader(GLenum type, std::string sourceCode)
{
    if(mBuildStatus)
    {
        Logger::getInstance()->logWarning("ShaderProgram: try to add new shader to an already built program ignored!");
        return false;
    }

    if(sourceCode == "")
    {
        Logger::getInstance()->logError("ShaderProgram::addNewShader(): source string is empty! No shader created!");
        return false;
    }

    auto shaderIterator = mShaderMap.find(type);
    if(mShaderMap.find(type) != mShaderMap.end())
    {
        Logger::getInstance()->logWarning("Try to add shader of type " + glutils::shaderTypeToString(type) + " to Shader Program which already contains shader of this type. Previous shader is overwritten!");
        glDetachShader(mShaderProgram, shaderIterator->second);
        glDeleteShader(shaderIterator->second);
    }

    GLuint shader = glCreateShader(type);

    const char* cSourceCode = sourceCode.c_str();
    glShaderSource(shader, 1, &cSourceCode, nullptr);
    glAttachShader(mShaderProgram, shader);

    mShaderMap[type] = shader;
    return true;
}

bool ShaderProgram::compileShader(GLuint shader, GLenum type)
{
    GLint status;
    GLchar infoLog[1024];
    GLsizei length;

    glCompileShaderIncludeARB(shader, 0, nullptr, nullptr);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

    if(status != GL_TRUE)
    {
        glGetShaderInfoLog(shader, 1024, &length, infoLog);
        Logger::getInstance()->logError("Compiling shader of type " + glutils::shaderTypeToString(type) + " failed:\n" + std::string(infoLog )+ "\n");
        return false;
    }

    return true;
}

bool ShaderProgram::link()
{
    GLint status;
    GLchar infoLog[1024];
    GLsizei length;

    glLinkProgram(mShaderProgram);
    glGetProgramiv(mShaderProgram, GL_LINK_STATUS, &status);

    if(status != GL_TRUE)
    {
        glGetProgramInfoLog(mShaderProgram, 1024, &length, infoLog);
        Logger::getInstance()->logError("Error linking shader progam:\n" + std::string(infoLog) + "\n");
        return false;
    }

    return true;
}


bool ShaderProgram::bindShaderStorageBuffer(const std::string& storageBlockName, BufferBase* buffer) const
{
    if(!bindBufferToShaderVariable(GL_SHADER_STORAGE_BUFFER, storageBlockName, buffer))
    {
//        Logger::getInstance()->logWarning("ShaderProgram: cannot bind shader storage buffer. No shader storage block named \"" + storageBlockName + "\".");
        return false;
    }

    return true;
}

bool ShaderProgram::bindUniformBuffer(const std::string& uniformBlockName, BufferBase* buffer) const
{
    if(!bindBufferToShaderVariable(GL_UNIFORM_BUFFER, uniformBlockName, buffer))
    {
        Logger::getInstance()->logWarning("ShaderProgram: cannot bind uniform buffer. No uniform block named \"" + uniformBlockName + "\".");
        return false;
    }
    return true;
}

bool ShaderProgram::bindAtomicCounterBuffer(const std::string& atomicCounterName, BufferBase* buffer) const
{
    if(!bindBufferToShaderVariable(GL_ATOMIC_COUNTER_BUFFER, atomicCounterName, buffer))
    {
        Logger::getInstance()->logWarning("ShaderProgram: canno bind atomic counter buffer. No atomic counter uniform named \"" + atomicCounterName + "\".");
        return false;
    }
    return true;
}


void ShaderProgram::querySubroutines(GLenum shaderStage)
{
    mSubroutineUniformCount[shaderStage] = 0;
    GLint* subroutineUniformCount = &(mSubroutineUniformCount[shaderStage]);
    glGetProgramStageiv(mShaderProgram, shaderStage, GL_ACTIVE_SUBROUTINE_UNIFORMS, subroutineUniformCount);
    mActiveSubroutines[shaderStage] = new GLuint[*subroutineUniformCount];

    // Set failsave defaults for all routines, i.e. activate the first compatible subroutine per subroutine uniform.
    GLint compatibleRoutineCount = 0;
    for(GLint i = 0; i < *subroutineUniformCount; ++i)
    {
        glGetActiveSubroutineUniformiv(mShaderProgram, shaderStage, i, GL_NUM_COMPATIBLE_SUBROUTINES, &compatibleRoutineCount);
        // Get compatible routines.
        GLint routines[compatibleRoutineCount];
        glGetActiveSubroutineUniformiv(mShaderProgram, shaderStage, i, GL_COMPATIBLE_SUBROUTINES, routines);
        mActiveSubroutines[shaderStage][i] = routines[0];
    }
}

void ShaderProgram::activateSubroutines() const
{
    GLsizei count = 0;

    for(auto routineIter : mActiveSubroutines)
    {
        count = mSubroutineUniformCount.find(routineIter.first)->second;

        if(count > 0)
            glUniformSubroutinesuiv(routineIter.first, count, routineIter.second);

//        Logger::getInstance()->logInfo("ShaderProgram: enabled " + std::to_string(count) + " subroutines for stage " + glutils::shaderTypeToString(routineIter.first));
    }
}

bool ShaderProgram::selectSubroutine(GLenum shaderStage, std::string selectorName, std::string subroutineName)
{
    GLint location = glGetSubroutineUniformLocation(mShaderProgram, shaderStage, selectorName.c_str());

    if(location == -1)
    {
        Logger::getInstance()->logWarning("ShaderProgram: selecting subroutine failed. No such subroutine selector: " + selectorName);
        return false;
    }

    GLint index = glGetSubroutineIndex(mShaderProgram, shaderStage, subroutineName.c_str());

    if(index == -1)
    {
        Logger::getInstance()->logWarning("ShaderProgram: selecting subroutine failed. No such subroutine: " + subroutineName);
        return false;
    }

//    Logger::getInstance()->logInfo("ShaderProgram: Set subroutine selector " + selectorName + " to use subroutine " + subroutineName + ".");

    mActiveSubroutines[shaderStage][location] = index;

    return true;
}

bool ShaderProgram::bindBufferToShaderVariable(GLenum bufferTarget, const std::string& shaderVariableName, BufferBase* buffer) const
{
    GLenum property = GL_BUFFER_BINDING;
    GLenum block;

    // Determine block name
    if(bufferTarget == GL_UNIFORM_BUFFER)
        block = GL_UNIFORM_BLOCK;
    else if(bufferTarget == GL_SHADER_STORAGE_BUFFER)
        block = GL_SHADER_STORAGE_BLOCK;
    else if(bufferTarget == GL_ATOMIC_COUNTER_BUFFER)
        block = GL_UNIFORM;

    // Get index of the block
    GLuint blockIndex = glGetProgramResourceIndex(mShaderProgram, block, shaderVariableName.c_str());

    if(blockIndex == GL_INVALID_INDEX)
        return false;

    // Special case for atomic counter buffers
    if(bufferTarget == GL_ATOMIC_COUNTER_BUFFER)
        block = GL_ATOMIC_COUNTER_BUFFER;

    // Determine binding index.
    GLint binding;
    glGetProgramResourceiv(mShaderProgram, block, blockIndex, 1, &property, 1, nullptr, &binding);

    // Bind buffer
    buffer->bindBase(bufferTarget, binding);

    return true;
}

} // namespace nparticles

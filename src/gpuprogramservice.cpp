/*
 * Copyright (C) 2015 Simon Kerler
 *
 * This file is part of the "Nameless Particle Engine".
 * For conditions of distribution and use, see the copyright notice you should
 * have recievied with this software. I not, see:
 * http://opensource.org/licenses/Zlib
 */

#include "gpuprogramservice.hpp"

#include "fileutils.hpp"
#include "logger.hpp"

namespace nparticles
{

GPUProgramService::GPUProgramService()
    : mDefaultRenderProgram(nullptr)
{
}

GPUProgramService::~GPUProgramService()
{
    for(auto renderIter : mRenderPrograms)
        delete renderIter.second;
    for(auto computeIter : mComputePrograms)
        delete computeIter.second;
}

RenderProgram* GPUProgramService::createRenderProgram(const std::string& id, const std::string& vertSrcFile, const std::string& fragSrcFile, const std::string& tcsSrcFile, const std::string& tesSrcFile, const std::string& geoSrcFile)
{
    if(mRenderPrograms.find(id) != mRenderPrograms.end())
    {
        Logger::getInstance()->logWarning("Render program with id \"" + id + "\" already exists!");
        return nullptr;
    }

    std::string tcsSource = "";
    std::string tesSource = "";
    std::string geoSource = "";

    if(tcsSrcFile.length() > 0)
        tcsSource = getSource(tcsSrcFile);
    if(tesSrcFile.length() > 0)
        tesSource = getSource(tesSrcFile);
    if(geoSrcFile.length() > 0)
        geoSource = getSource(geoSrcFile);

    RenderProgram* renderProgram = new RenderProgram(
                getSource(vertSrcFile),
                getSource(fragSrcFile),
                tcsSource,
                tesSource,
                geoSource);

    if(!renderProgram->build())
    {
        Logger::getInstance()->logError("Compiling render program \"" + id + "\" failed.");
        delete renderProgram;
        return nullptr;
    }

    mRenderPrograms[id] = renderProgram;

    Logger::getInstance()->logInfo("Created render program \"" + id + "\".");

    return renderProgram;
}

RenderProgram* GPUProgramService::getRenderProgram(const std::string& id)
{
    if(mRenderPrograms.find(id) == mRenderPrograms.end())
        return nullptr;
    return mRenderPrograms[id];
}

RenderProgram* GPUProgramService::getDefaultRenderProgram()
{
    // Create the default render program if necessary
    if(!mDefaultRenderProgram)
    {
        std::string vertexSource =
                "#version 430\n"
                "in vec3 np_in_position;\n"
                "uniform mat4 np_viewProjectionMatrix;\n"
                "\n"
                "void main()\n"
                "{\n"
                "    gl_PointSize = 3.0;\n"
                "    gl_Position = np_viewProjectionMatrix * vec4(np_in_position, 1.0);\n"
                "}\n"
                ;

        std::string fragmentSource =
                "#version 430\n"
                "out vec4 np_fColor;\n"
                "\n"
                "void main()\n"
                "{\n"
                "    np_fColor = vec4(1.0, 0.0, 0.0, 1.0);\n"
                "}\n"
                ;

        mDefaultRenderProgram = new RenderProgram(vertexSource, fragmentSource, "", "", "");
        mDefaultRenderProgram->build();
    }

    return mDefaultRenderProgram;
}

ComputeProgram* GPUProgramService::createComputeProgram(const std::string& id, const std::string& srcFile)
{
    if(mComputePrograms.find(id) != mComputePrograms.end())
    {
        Logger::getInstance()->logWarning("Compute program with id \"" + id + "\" already exists!");
        return nullptr;
    }

    ComputeProgram* computeProgram = new ComputeProgram(getSource(srcFile));

    if(!computeProgram->build())
    {
        Logger::getInstance()->logError("Compiling compute program \"" + id + "\" failed.");
        delete computeProgram;
        return nullptr;
    }

    mComputePrograms[id] = computeProgram;

    Logger::getInstance()->logInfo("Created compute program \"" + id + "\".");

    return computeProgram;
}

ComputeProgram* GPUProgramService::getComputeProgram(const std::string& id)
{
    if(mComputePrograms.find(id) == mComputePrograms.end())
        return nullptr;
    return mComputePrograms[id];
}

bool GPUProgramService::addSourceDirectory(const std::string& _sourceDirectoryPath, const std::string& _destination, bool recursive)
{
    std::string destination = _destination;
    std::string sourceDirectoryPath = _sourceDirectoryPath;

    if(!fileutils::isDirectory(sourceDirectoryPath))
    {
        Logger::getInstance()->logWarning("Cannot add directory to GLSL source files. Directory \"" + sourceDirectoryPath + "\" does not exist or is no directory.");
        return false;
    }

    // Append missing slash
    if(destination.back() != '/')
        destination += "/";

    if(sourceDirectoryPath.back() != '/')
        sourceDirectoryPath += "/";

    fileutils::directory_files directoryFiles = fileutils::getDirectoryFiles(sourceDirectoryPath, recursive);
    for(auto file : directoryFiles)
        addSourceFile(sourceDirectoryPath + file, destination + file);

    return true;
}

bool GPUProgramService::addSourceFile(const std::string& sourceFilePath, const std::string& destination)
{
    if(!fileutils::isFile(sourceFilePath))
    {
        Logger::getInstance()->logWarning("Cannot add GLSL source file. File \"" + sourceFilePath + "\" does not exist or is no valid file.");
        return false;
    }

    // Build destination name
    std::string destName = destination;
    if(destName.back() == '/')
        destName += fileutils::getFileName(sourceFilePath);

    std::string source = fileutils::readFile(sourceFilePath);
    glNamedStringARB(GL_SHADER_INCLUDE_ARB, -1, destName.c_str(), -1, source.c_str());

    Logger::getInstance()->logInfo("Added GLSL source file \"" + sourceFilePath + "\" as \"" + destName + "\".");
    return true;
}

std::string GPUProgramService::getSource(const std::string& sourceName)
{
    if(!glIsNamedStringARB(-1, sourceName.c_str()))
    {
        Logger::getInstance()->logWarning("No such GLSL source file: " + sourceName);
        return "";
    }

    int sourceLength = 0;
    glGetNamedStringivARB(-1, sourceName.c_str(), GL_NAMED_STRING_LENGTH_ARB, &sourceLength);

    GLchar sourceString[sourceLength];
    glGetNamedStringARB(-1, sourceName.c_str(), sourceLength, nullptr, sourceString);

    return std::string(sourceString);
}

} // namespace nparticles

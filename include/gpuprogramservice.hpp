/*
 * Copyright (C) 2015 Simon Kerler
 *
 * This file is part of the "Nameless Particle Engine".
 * For conditions of distribution and use, see the copyright notice you should
 * have recievied with this software. I not, see:
 * http://opensource.org/licenses/Zlib
 */

#ifndef NP_GPUPROGRAMSERVICE_HPP
#define NP_GPUPROGRAMSERVICE_HPP

#include <map>

#include "renderprogram.hpp"
#include "computeprogram.hpp"

namespace nparticles
{

/**
 * The GPUProgramService class manages RenderProgram and ComputeProgram objects.
 *
 * The GPUProgramService is similar to ResourceManagers to manage RenderProgram and CmoputeProgram objects.
 * It provides an interface to create and retrieve ShaderPrograms.
 *
 * @note The Engine holds an instance of GPUProgramService so this instance should be used. It is not recommended to
 * create a separate GPUProgramService.
 *
 * The GPUProgramService also handles source "files". Source files are named strings (provided by the GL_ARB_shading_language_include
 * extension) which reside in a virtual file system.
 *
 * @note All paths in the virtual file system have to start with "/". This equals absolute unix file system paths.
 *
 * To add a new source file to the file system, addSourceFile() is used. To add a whole directory of files, addSourceDirectory() is used.
 * For details for naming and addressing a source file, see the documentation of this two methods.
 *
 * @note At all occurences, the term "source file" refers to a virtual file residing in the virtual file system.
 */
class GPUProgramService
{
friend class Engine;

public:
    /**
     * Create a new RenderProgram.
     *
     * This method creates a new RenderProgram managed by the GPUProgramService.
     *
     * The source code is read from the specified source files residing in the virtual file system. This source code is passed
     * to the RenderProgram constructor which should be consulted for detailed description.
     *
     * @param id The ID of the new RenderProgram.
     * @param vertSrcFile Virtual source file for vertex shader source code.
     * @param fragSrcFile Virtual source file for fragment shader source code.
     * @param tcsSrcFile Virtual source file for tesselation control shader source code.
     * @param tesSrcFile Virtual source file for tesselation evaluation shader source code.
     * @param geoSrcFile Virtual source file for geometry shader source code.
     *
     * @return Pointer to the newly created RenderProgram. Null pointer if a RenderProgram with ID @id already exists or if the RenderProgram
     *         could not be built without errors (ShaderProgram::build()).
     */
    RenderProgram* createRenderProgram(const std::string& id, const std::string& vertSrcFile, const std::string& fragSrcFile,
                                       const std::string& tcsSrcFile = "", const std::string& tesSrcFile = "", const std::string& geoSrcFile = "");

    /**
     * Get a RenderProgram by its ID.
     *
     * This retrieves a RenderProgram specified by its ID.
     *
     * @param id String ID of the RenderProgram.
     *
     * @return Pointer to the RenderProgram or null pointer if no RenderProgram with ID @p id was created by createRenderProgram().
     */
    RenderProgram* getRenderProgram(const std::string& id);

    /**
     * Get the default RenderProgram.
     *
     * This returns a default RenderProgram which can be used if no custom RenderProgram is specified. The default RenderProgram is created
     * when getDefaultRenderProgram() is called the first time.
     *
     * @return Pointer to the default RenderProgram.
     */
    RenderProgram* getDefaultRenderProgram();

    /**
     * Create a new ComptueProgram.
     *
     * This method creates a new ComputeProgram managed by the GPUProgramService.
     *
     * The source code is read from the specified source file residing in the virtual file system. This source code is passed to the
     * ComptueProgram constructor which schould be consulted for detailed description.
     *
     * @param id The ID of the new ComputeProgram.
     * @param srcFile Virtual source file for compute shader soruce code.
     *
     * @return Pointer to the newly created ComputeProgram. Null pointer if a ComputeProgram with ID @id already exists or if the ComputeProgram
     *         could not be built without errors (ShaderProgram::build()).
     */
    ComputeProgram* createComputeProgram(const std::string& id, const std::string& srcFile);

    /**
     * Get a RenderProgram by its ID.
     *
     * This retrieves a ComputeProgram specified by its ID.
     *
     * @param id String ID of the ComputeProgram.
     *
     * @return Pointer to the ComputeProgram or null pointer if no RenderProgram with ID @p id was created by createComputeProgram().
     */
    ComputeProgram* getComputeProgram(const std::string& id);

    /**
     * Add a files of a directory to the virtual file system.
     *
     * This adds the files of directory @p sourceDirectoryPath into the @p destination directory of the virtual file system. If @p recursive
     * is true, all subdirectories of @p sourceDirectoryPath are also added.
     *
     * Examples:
     *
     * @code
     * "real" file system structure on your hard drive:
     * /
     *  |
     *  + root.glsl
     *  + foo
     *    |
     *    + oof.glsl
     *    + bar
     *      |
     *      + baz.glsl
     *      + baru.glsl
     *
     * command                                          created virtual files    content of
     * addSourceDirectory("/foo/bar", "/", false)  =>   /baz.glsl                /foo/bar/baz.glsl
     *                                                  /baru.glsl               /foo/bar/baru.glsl
     *
     * addSourceDirectory("/foo", "/derp", true)   =>   /derp/oof.glsl           /foo/oof.glsl
     *                                                  /derp/bar/baz.glsl       /foo/bar/baz.glsl
     *                                                  /derp/bar/baru.glsl      /foo/bar/bar.glsl
     * @endcode
     *
     * @param sourceDirectoryPath The path to the directory of sources (on your hard drive) to be added.
     * @param destination A vritual destination directory.
     * @param recursive Set to true if @p sourceDirectoryPath should be added recursively.
     *
     * @return True if files added successfully. False if @p sourceDirectoryPath is no directory or does not exists.
     */
    bool addSourceDirectory(const std::string& sourceDirectoryPath, const std::string& destination, bool recursive = false);

    /**
     * Add a file to the virtual file system.
     *
     * This adds a single file @p sourceFilePath to @p destination in the virtual file system. If @p destination ends with "/" it is interpreted
     * as a directory and the original filename is appended. If it does not end with "/" it is interpreted as a destination file name:
     *
     * @code
     * command                                        created virtual file
     * addSourceFile("/foo/bar.glsl", "/baru")   =>   /baru
     * addSourceFile("/foo/bar.glsl", "/baru/")  =>   /baru/bar.glsl
     * @endcode
     *
     * @param sourceFilePath The path to the source file (on your hard drive) to be added.
     * @param destination The virtual destination directory or file.
     *
     * @return True if file was added successfully. False if @p sourceFilePath is no file or does not exists.
     */
    bool addSourceFile(const std::string& sourceFilePath, const std::string& destination);

private:
    /**
     * GPUProgramService constructor.
     */
    GPUProgramService();

    /**
     * GPUProgramService desturctor.
     */
    ~GPUProgramService();

    /**
     * Helper method to get file content from a virtual file.
     *
     * This method returns the content of a file of the virtual file system.
     *
     * @param sourceName The name of the virtual file.
     *
     * @return The content of the virtual file or an empty string, if the file does not exist.
     */
    std::string getSource(const std::string& sourceName);

    /**
     * Map used to manage RenderPrograms.
     *
     * This map stores all created RenderPrograms. Key is the ID of the program and value a pointer to the associated RenderProgram.
     */
    std::map<const std::string, RenderProgram*> mRenderPrograms;

    /**
     * This is a pointer to the default RenderProgram.
     *
     * This is the default RenderProgram created when getDefaultRenderProgram() is called the first time.
     */
    RenderProgram* mDefaultRenderProgram;

    /**
     * Map used to manage ComptuePrograms.
     *
     * This map stores all created ComputePrograms. Key is the ID of the program and value a pointer to the associated ComputeProgram.
     */
    std::map<const std::string, ComputeProgram*> mComputePrograms;

    // Hide copy constructor and assignment operator
    GPUProgramService(const GPUProgramService&) = delete;
    void operator=(const GPUProgramService&) = delete;
};

} // namespace particles

#endif // NP_GPUPROGRAMSERVICE_HPP

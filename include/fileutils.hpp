/*
 * Copyright (C) 2015 Simon Kerler
 *
 * This file is part of the "Nameless Particle Engine".
 * For conditions of distribution and use, see the copyright notice you should
 * have recievied with this software. I not, see:
 * http://opensource.org/licenses/Zlib
 */

#ifndef NP_FILEUTILS_HPP
#define NP_FILEUTILS_HPP

#include <iostream>
#include <vector>

namespace nparticles
{

/**
 * The fileutils namesapce contains helper methods for file system operations.
 */
namespace fileutils
{

/**
 * @brief Read a whole file and return its content.
 * @param filePath The path (absolute or relative) to the file to read.
 * @return String containing the full file content.
 */
std::string readFile(const std::string& filePath);

/**
 * @brief Check if given path is a file (not directory).
 * @param path The relative or absolute path to a file.
 * @return True if path exists and is a file.
 */
bool isFile(const std::string& path);

/**
 * @brief Check if given path is a directory (not file).
 * @param path The relative or absolute path to a directory.
 * @return True if path exists and is a directory.
 */
bool isDirectory(const std::string& path);

/**
 * @brief Return the filename of a path, i.e. omit the directories.
 * @param path Full path (relative or absolute) to a file.
 * @return The filename only.
 */

std::string getFileName(const std::string& path);

/**
 * Typedef for a files of a directory.
 *
 * A vector containing the names of files.
 */
typedef std::vector<std::string> directory_files;

/**
 * @brief Get the files (not directories) contained in given directory.
 *
 * Returns the relative paths of files located in given directory. A leading "./" is omitted as well as the given directory name.
 * Names of subdirectories (if called recursively) are added (see parameter parent).
 *
 * Subdirectories are not added to the list of files.
 *
 * @param directory The path to a directory.
 * @param recursive Whether a recursive look up should be performed or not.
 * @param parent The parent (or simple: prefix) which is added in front of the returned file names. Mostly only used by
 *               a recursive call and not for user specification.
 * @return
 */
directory_files getDirectoryFiles(const std::string& directory, bool recursive = false, const std::string& parent = "");

} // namespace fileutils
} // namespace nparticles

#endif // NP_FILEUTILS_HPP

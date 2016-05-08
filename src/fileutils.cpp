/*
 * Copyright (C) 2015 Simon Kerler
 *
 * This file is part of the "Nameless Particle Engine".
 * For conditions of distribution and use, see the copyright notice you should
 * have recievied with this software. I not, see:
 * http://opensource.org/licenses/Zlib
 */

#include "fileutils.hpp"

#include <fstream>

#define BOOST_FILESYSTEM_NO_DEPRECATED
#include <boost/filesystem.hpp>

namespace bfs = boost::filesystem;

namespace nparticles
{
namespace fileutils {

std::string readFile(const std::string& filePath)
{
    std::ifstream t(filePath);

    std::string str(
        (std::istreambuf_iterator<char>(t)),
        std::istreambuf_iterator<char>()
    );

    return str;
}

bool isFile(const std::string& path)
{
    return bfs::exists(path) && bfs::is_regular_file(path);
}

bool isDirectory(const std::string& path)
{
    return bfs::exists(path) && bfs::is_directory(path);
}

std::string getFileName(const std::string& path)
{
    return bfs::path(path).filename().native();
}

directory_files getDirectoryFiles(const std::string& directory, bool recursive, const std::string& parent)
{
    directory_files files;

    bfs::directory_iterator dirIter(directory);
    bfs::directory_iterator endIter;

    for(; dirIter != endIter; ++dirIter)
    {
        if(bfs::is_directory(*dirIter) && recursive)
        {
            auto subFiles = getDirectoryFiles((*dirIter).path().native(), true, parent + (*dirIter).path().filename().native() + "/");
            files.insert(files.end(), subFiles.begin(), subFiles.end());
        }
        else if(bfs::is_regular_file(*dirIter))
        {
            files.push_back(parent + (*dirIter).path().filename().native());
        }
    }
    return files;
}


} // namespace fileutils
} // namespace nparticles

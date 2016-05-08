/*
 * Copyright (C) 2015 Simon Kerler
 *
 * This file is part of the "Nameless Particle Engine".
 * For conditions of distribution and use, see the copyright notice you should
 * have recievied with this software. I not, see:
 * http://opensource.org/licenses/Zlib
 */

#ifndef NP_RESOURCEMANAGER_HPP
#define NP_RESOURCEMANAGER_HPP

#include <string>
#include <map>

namespace nparticles
{

/**
 * Class for managing resources.
 *
 * This class is used to manage resources like Meshes or Materials. Each resource has an unique string
 * identifier which can be used to retrieve this very object from the manager.
 *
 * @tparam T This is the class of the resource to manage, e.g. Mesh or Material.
 */
template<class T>
class ResourceManager
{
public:
    /**
     * The ResourceManager constructor.
     */
    ResourceManager()
        : mDefaultResource(nullptr)
    {
    }

    /**
     * The ResourceManager destructor.
     *
     * This frees all resources managed by this ResourceManager, so all pointers or references to the
     * managed resources become invalid after the ResourceManager was destroyed!
     *
     * This is a good reason, why ResourceManagers should be made Singleton, so they are only destroyed
     * after application termination.
     */
    virtual ~ResourceManager()
    {
        for(auto resourceIter : mResourceMap)
            delete resourceIter.second;
        delete mDefaultResource;
    }

    /**
     * Retrieve a managed resource.
     *
     * This queries the managed resoruces for a resoruce with @p id. This resource is returned
     * when found or a null pointer is returned, if no resource has @p id.
     *
     * @param id String ID of the resource which is requested.
     *
     * @return Pointer to the resource or null pointer if tehre is no resource named @id.
     */
    const T* getResource(std::string id)
    {
        if(exists(id))
            return mResourceMap[id];
        return nullptr;
    }

    /**
     * This returns the special default resource.
     *
     * The default resource is provided as a fallback, if no apporpriate resource can be found.
     *
     * @return Pointer to the default resource.
     */
    const T* getDefaultResource()
    {
        if(!mDefaultResource)
            createDefaultResource();
        return mDefaultResource;
    }

    /**
     * Check if a resource with ID @p id exists.
     *
     * This returns whether a resource with @p id exists or not.
     *
     * @param id String ID of a resource.
     *
     * @return True, if a resource with ID @p id exists, false otherwise.
     */
    bool exists(std::string id)
    {
        return (mResourceMap.find(id) != mResourceMap.end());
    }

protected:
    /**
     * Pure virtual function used to create the default resource.
     *
     * This method is called by ResourceManager::getDefaultResource() if the
     * default resource has not yet been created. So this method is called only
     * once.
     */
    virtual void createDefaultResource() = 0;

    // Hide copy constructor and assignment operator
    ResourceManager(const ResourceManager&) = delete;
    void operator=(const ResourceManager&) = delete;

    /**
     * Map containing all managed resources excpet the default resource.
     *
     * Key is the string ID of the resoruce, value pointer to the resource.
     */
    std::map<std::string, T*> mResourceMap;

    /**
     * Pointer to the default resource.
     *
     * The default resource is created by calling ResourceManager::getDefaultResource() the first time.
     */
    T* mDefaultResource;
};

} // namespace nparticles

#endif // NP_RESOURCEMANAGER_HPP

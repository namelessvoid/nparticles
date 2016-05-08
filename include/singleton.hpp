/*
 * Copyright (C) 2015 Simon Kerler
 *
 * This file is part of the "Nameless Particle Engine".
 * For conditions of distribution and use, see the copyright notice you should
 * have recievied with this software. I not, see:
 * http://opensource.org/licenses/Zlib
 */

#ifndef NP_SINGLETON_HPP
#define NP_SINGLETON_HPP

namespace nparticles
{

/**
 * A generic Singleton class to easily create singletons.
 *
 * @par Usage
 * To ensure, the converted class can not be instantiated, change its constructor,
 * copy constructor and assignment operators to private and make it a friend of Singleton<ClassName>
 * so Singelton has access to these methods.
 *
 * @tparam T The class that is converted to singleton.
 */
template<typename T>
class Singleton
{
public:
    /**
     * Get the singleton instance.
     *
     * This returns a pointer to the one instance of class T. If requested the first time, T's constructor is called
     * to create a new instance. Proceeding calls to getInstance() always return the same object.
     *
     * @return Pointer to the unique instance of T.
     */
    static T* getInstance()
    {
        if(!mSingleton)
            Singleton<T>::mSingleton = new T();

        return mSingleton;
    }


protected:
    /**
     * Singleton constructor.
     */
    Singleton()
    {
        mSingleton = static_cast<T*>(this);
    }

    /**
     * Singleton destructor.
     */
    ~Singleton() {}

    /**
     * Pointer to the unique instance of Singleton<T>.
     */
    static T* mSingleton;

private:
    // Hide copy constructor and assignment operators
    Singleton(const Singleton<T> &) = delete;
    Singleton& operator=(const Singleton<T> &) = delete;
};

// Initialize the singleton pointer to null.
template<typename T> T* Singleton<T>::mSingleton = nullptr;

} // namespace nparticles

#endif // NP_SINGLETON_HPP

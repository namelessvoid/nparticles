/*
 * Copyright (C) 2015 Simon Kerler
 *
 * This file is part of the "Nameless Particle Engine".
 * For conditions of distribution and use, see the copyright notice you should
 * have recievied with this software. I not, see:
 * http://opensource.org/licenses/Zlib
 */

#ifndef NP_LOGGER_HPP
#define NP_LOGGER_HPP

#include <string>
#include <iostream>

#include "singleton.hpp"

namespace nparticles
{

/**
 * A basic Logger class.
 *
 * This basic Logger class can be used to log to stdout and stderr.
 */
class Logger : public Singleton<Logger>
{
friend class Singleton<Logger>;

public:
    /**
     * @brief Log a info message.
     * @param message The info message to log.
     */
    inline void logInfo(std::string message)
    {
        if(!disabled)
            std::cout << "Info: " << message << std::endl;
    }

    /**
     * @brief Log a warning.
     * @param message The warning message to log.
     */
    inline void logWarning(std::string message)
    {
        if(!disabled)
            std::cerr << "Warning: " << message << std::endl;
    }

    /**
     * @brief Log an error.
     * @param message The error message to log.
     */
    inline void logError(std::string message)
    {
        if(!disabled)
            std::cerr << "ERROR: " <<  message << std::endl;
    }

    /**
     * Disable the logger.
     *
     * @param disabled Set to true to disable logging.
     */
    inline void disable(bool disabled)
    {
        this->disabled = disabled;
    }

protected:
    /**
     * @brief Standard constructor which does nothing.
     */
    Logger() {}

    /**
     * @brief Standard destructor which does nothing.
     */
    ~Logger() {}

    /**
     * Whether the logger is disabled or not.
     */
    bool disabled;

    // Hide copy constructor and assignment operator
    Logger(const Logger&) = delete;
    void operator=(const Logger&) = delete;
};

} // namespace nparticles

#endif // NP_LOGGER_HPP

/*
** EPITECH PROJECT, 2024
** RTYPE-mirror
** File description:
** NetworkWrapper.cpp
*/

#include <spdlog/spdlog.h>

#include "network/NetworkWrapper.hpp"

bool network::NetworkWrapper::initialized = false;

/**
 * @brief Initializes the ENet library.
 *
 * This method initializes the ENet library, making it ready for use.
 * If the library is already initialized, it does nothing and returns true.
 *
 * @return True if the initialization is successful or already completed,
 * false if the initialization fails.
 */
bool network::NetworkWrapper::initialize()
{
    if (initialized)
        return true;

    if (enet_initialize() != 0)
    {
        spdlog::error("Network initialization failed!");
        return false;
    }

    initialized = true;
    return true;
}

/**
 * @brief Shuts down the ENet library.
 *
 * This method deinitializes the ENet library if it has been initialized,
 * releasing any resources associated with it. If the library is not
 * initialized, it does nothing.
 */
void network::NetworkWrapper::shutdown()
{
    if (initialized)
    {
        enet_deinitialize();
        initialized = false;
    }
}

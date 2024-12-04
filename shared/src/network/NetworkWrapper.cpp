/*
** EPITECH PROJECT, 2024
** RTYPE-mirror
** File description:
** NetworkWrapper.cpp
*/

#include <spdlog/spdlog.h>

#include "network/NetworkWrapper.hpp"

bool network::NetworkWrapper::initialized = false;

// Initialize ENet
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

// Shutdown ENet
void network::NetworkWrapper::shutdown()
{
    if (initialized)
    {
        enet_deinitialize();
        initialized = false;
    }
}

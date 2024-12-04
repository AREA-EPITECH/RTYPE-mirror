/*
** EPITECH PROJECT, 2024
** RTYPE-mirror
** File description:
** NetworkWrapper.hpp
*/

#pragma once

#include <enet/enet.h>

namespace network
{
    class NetworkWrapper
    {
    public:
        static bool initialized;
        static bool initialize();
        static void shutdown();
    };
}  // namespace network

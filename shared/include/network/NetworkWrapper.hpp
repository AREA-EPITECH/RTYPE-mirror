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
    /**
     * @class NetworkWrapper
     * @brief Utility class for initializing and shutting down the ENet networking library.
     *
     * The NetworkWrapper class provides static methods to manage the lifecycle
     * of the ENet library. It ensures that the library is properly initialized
     * and deinitialized, maintaining a global state to prevent redundant operations.
     *
     * @namespace network
     */
    class NetworkWrapper
    {
    public:
        static bool initialized; ///< Indicates whether the ENet library has been initialized.

        /**
         * @brief Initializes the ENet library.
         *
         * This method initializes the ENet library, making it ready for use.
         * If the library is already initialized, it does nothing and returns true.
         *
         * @return True if the initialization is successful or already completed,
         * false if the initialization fails.
         */
        static bool initialize();

        /**
         * @brief Shuts down the ENet library.
         *
         * This method deinitializes the ENet library if it has been initialized,
         * releasing any resources associated with it. If the library is not
         * initialized, it does nothing.
         */
        static void shutdown();
    };
} // namespace network

/*
** EPITECH PROJECT, 2024
** RTYPE-mirror
** File description:
** PeerWrapper.hpp
*/

#pragma once

#include <any>
#include <enet/enet.h>
#include <memory>

namespace network
{
    /**
     * @class PeerWrapper
     * @brief Wrapper class for managing ENetPeer instances and associated user data.
     *
     * The PeerWrapper class encapsulates an ENetPeer pointer, providing
     * safe memory management and the ability to associate arbitrary data
     * with a peer using `std::any`. This allows additional information
     * about a peer to be stored and retrieved dynamically.
     *
     * @namespace network
     */
    class PeerWrapper
    {
    private:
        std::shared_ptr<ENetPeer> _peer; ///< Shared pointer to the ENetPeer instance.
        std::any _data; ///< Container for storing user-defined data associated with the peer.

    public:
        /**
         * @brief Constructs a PeerWrapper instance from a raw ENetPeer pointer.
         *
         * Wraps the given ENetPeer pointer in a shared pointer for safe memory management.
         *
         * @param rawPeer A raw pointer to the ENetPeer instance.
         */
        PeerWrapper(ENetPeer *rawPeer) : _peer(rawPeer, [](ENetPeer *p) {}) {};

        /**
         * @brief Retrieves the underlying ENetPeer instance.
         *
         * Provides access to the shared pointer wrapping the ENetPeer.
         *
         * @return A shared pointer to the ENetPeer.
         */
        std::shared_ptr<ENetPeer> getPeer() const { return _peer; }

        /**
         * @brief Sets user-defined data associated with the peer.
         *
         * Uses `std::any` to store data of any type associated with the peer.
         *
         * @tparam T The type of the data to store.
         * @param data The data to associate with the peer.
         */
        template <typename T>
        void setData(T &&data)
        {
            _data = std::forward<T>(data);
        }

        /**
         * @brief Retrieves user-defined data associated with the peer.
         *
         * Casts the stored data to the specified type and returns it.
         *
         * @tparam T The expected type of the stored data.
         * @return The data associated with the peer.
         * @throws std::bad_any_cast If the type of the stored data does not match T.
         */
        template <typename T>
        T getData() const
        {
            return std::any_cast<T>(_data);
        }

        /**
         * @brief Checks if any user-defined data is associated with the peer.
         *
         * @return True if data is associated with the peer, false otherwise.
         */
        bool hasData() const { return _data.has_value(); }
    };

} // namespace network

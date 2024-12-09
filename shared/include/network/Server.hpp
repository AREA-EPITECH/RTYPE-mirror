/*
** EPITECH PROJECT, 2024
** RTYPE-mirror
** File description:
** Server.hpp
*/

#pragma once

#include <cstdint>
#include <enet/enet.h>
#include <memory>
#include <unordered_map>
#include <vector>

#include "PeerWrapper.hpp"
#include "event/Event.hpp"
#include "packet/Packet.hpp"

namespace network
{
    /**
     * @class NetworkServer
     * @brief Handles network communication for a server using the ENet library.
     *
     * The NetworkServer class is responsible for setting up, managing, and
     * handling network communication for a server. It uses ENet to handle
     * low-level networking and provides a higher-level abstraction for
     * sending packets and processing events.
     *
     * @namespace network
     */
    class NetworkServer
    {
    private:
        std::unique_ptr<ENetHost, decltype(&enet_host_destroy)> host; ///< ENet host for managing network connections.
        std::unordered_map<ENetPeer *, std::shared_ptr<PeerWrapper>> peerMap; ///< Map of the peer connections.
        uint16_t lastPacketId = 0; ///< Tracks the ID of the last packet sent.

    public:
        /**
         * @brief Constructs a new NetworkServer instance.
         *
         * Initializes the internal ENet host and other required resources.
         */
        NetworkServer();

        /**
         * @brief Destroys the NetworkServer instance.
         *
         * Cleans up the ENet host and other allocated resources.
         */
        ~NetworkServer();

        /**
         * @brief Starts the server and binds it to a specific port.
         *
         * @param port The port number to bind the server to.
         * @return True if the server starts successfully, false otherwise.
         */
        bool start(uint16_t port);

        /**
         * @brief Stops the server and closes all active connections.
         */
        void stop();

        /**
         * @brief Sends a raw packet of data to a specific peer.
         *
         * @param data The data to send.
         * @param peer A shared pointer to the target PeerWrapper.
         * @return True if the packet is sent successfully, false otherwise.
         */
        bool sendPacket(const std::vector<uint8_t> &data, std::shared_ptr<PeerWrapper> peer);

        /**
         * @brief Sends a snapshot packet to a specific peer.
         *
         * @param packet The snapshot packet to send.
         * @param peer A shared pointer to the target PeerWrapper.
         * @return True if the packet is sent successfully, false otherwise.
         */
        bool sendSnapshotPacket(const SnapshotPacket &packet, std::shared_ptr<PeerWrapper> peer);

        /**
         * @brief Sends a lobby snapshot packet to a specific peer.
         *
         * @param packet The lobby snapshot packet to send.
         * @param peer A shared pointer to the target PeerWrapper.
         * @return True if the packet is sent successfully, false otherwise.
         */
        bool sendLobbyPacket(const LobbySnapshotPacket &packet, std::shared_ptr<PeerWrapper> peer);

        /**
         * @brief Polls the server for new network events.
         *
         * Processes connection, disconnection, and data reception events,
         * populating the ServerEvent object with the event details.
         *
         * @param event A reference to a ServerEvent object to store the next event.
         * @return True if an event is successfully polled, false otherwise.
         */
        bool pollEvent(ServerEvent &event);
    };
} // namespace network

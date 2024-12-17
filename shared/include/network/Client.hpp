/*
** EPITECH PROJECT, 2024
** RTYPE-mirror
** File description:
** Client.hpp
*/

#pragma once

#include <cstdint>
#include <enet/enet.h>
#include <memory>
#include <vector>

#include "PeerWrapper.hpp"
#include "event/Event.hpp"
#include "packet/Packet.hpp"

namespace network
{
    /**
     * @class NetworkClient
     * @brief Manages the connection and communication with a server using ENet.
     *
     * The NetworkClient class handles client-side networking functionality,
     * including connecting to a server, sending various types of packets, and
     * polling for incoming events. It encapsulates the ENet library functionality,
     * ensuring proper initialization and cleanup.
     *
     * @namespace network
     */
    class NetworkClient
    {
    private:
        std::unique_ptr<ENetHost, decltype(&enet_host_destroy)> client; ///< ENet client host.
        std::unique_ptr<PeerWrapper> serverPeer; ///< Wrapper for the server peer connection.
        uint16_t lastPacketId = 0; ///< Tracks the ID of the last packet sent.

    public:
        /**
         * @brief Constructs a NetworkClient instance.
         *
         * Initializes the ENet library and prepares the client for use.
         */
        NetworkClient();

        /**
         * @brief Destroys the NetworkClient instance.
         *
         * Disconnects from the server if connected and shuts down the ENet library.
         */
        ~NetworkClient();

        /**
         * @brief Connects to a server.
         *
         * Attempts to establish a connection to the specified server address and port.
         *
         * @param host The server's hostname or IP address.
         * @param port The server's port number.
         * @return True if the connection is successful, false otherwise.
         */
        bool connectToServer(const std::string &host, uint16_t port);

        /**
         * @brief Disconnects from the server.
         *
         * Closes the connection to the server and releases associated resources.
         */
        void disconnect();

        /**
         * @brief Sends a raw packet to the server.
         *
         * @param data The binary data to send.
         * @return True if the packet is sent successfully, false otherwise.
         */
        bool sendPacket(const std::vector<uint8_t> &data);

        /**
         * @brief Sends an input packet to the server.
         *
         * @param packet The InputPacket containing user input data.
         * @return True if the packet is sent successfully, false otherwise.
         */
        bool sendInputPacket(const struct InputPacket &packet);

        /**
         * @brief Sends a lobby action packet to the server.
         *
         * @param packet The LobbyActionPacket containing lobby action details.
         * @return True if the packet is sent successfully, false otherwise.
         */
        bool sendLobbyPacket(const struct LobbyActionPacket &packet);

        /**
         * @brief Polls for incoming events from the server.
         *
         * Checks for incoming events such as data reception or server disconnection.
         * Populates the provided ClientEvent with event details if an event is received.
         *
         * @param event The ClientEvent structure to populate with event details.
         * @return True if an event is successfully polled, false otherwise.
         */
        bool pollEvent(ClientEvent &event);
    };
} // namespace network

/*
** EPITECH PROJECT, 2024
** RTYPE-mirror
** File description:
** NetworkWrapper.hpp
*/

#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include <enet/enet.h>
#include "Packet.hpp"

namespace network
{
    class NetworkWrapper
    {
    public:
        static bool initialized;
        static bool initialize();
        static void shutdown();
    };

    // Server Events
    enum class ServerEventType {
        ClientConnect,
        ClientDisconnect,
        DataReceive
    };

    struct ServerEvent {
        ServerEventType type;
        ENetPeer* peer;
        std::vector<uint8_t> data; // Only valid for DataReceive events
    };

    // Client Events
    enum class ClientEventType {
        ServerDisconnect,
        DataReceive
    };

    struct ClientEvent {
        ClientEventType type;
        SnapshotPacket data; // Only valid for DataReceive events
    };

    class NetworkServer
    {
    private:
        ENetHost* host;

    public:
        NetworkServer();
        ~NetworkServer();

        bool start(uint16_t port);
        void stop();
        bool sendPacket(const std::vector<uint8_t>& data, ENetPeer* peer);
        bool pollEvent(ServerEvent& event);
    };

    class NetworkClient
    {
    private:
        ENetHost* client;
        ENetPeer* serverPeer;

    public:
        NetworkClient();
        ~NetworkClient();

        bool connectToServer(const std::string& host, uint16_t port);
        void disconnect();
        bool sendPacket(const std::vector<uint8_t>& data);
        bool pollEvent(ClientEvent& event);
    };
}  // namespace network

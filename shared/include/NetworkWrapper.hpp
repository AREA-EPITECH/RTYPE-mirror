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

namespace network
{
    class NetworkWrapper
    {
    public:
        static bool initialized;
        static bool initialize();
        static void shutdown();
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
        void processEvents();
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
        void processEvents();
    };
}  // namespace network

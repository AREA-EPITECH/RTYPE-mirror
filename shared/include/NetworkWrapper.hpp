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
#include <unordered_map>

#include <enet/enet.h>
#include "Packet.hpp"
#include "PeerWrapper.hpp"

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
        std::shared_ptr<PeerWrapper> peer;
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
        std::unique_ptr<ENetHost, decltype(&enet_host_destroy)> host;
        std::unordered_map<ENetPeer*, std::shared_ptr<PeerWrapper>> peerMap;
        uint16_t lastPacketId = 0;

    public:
        NetworkServer();
        ~NetworkServer();

        bool start(uint16_t port);
        void stop();
        bool sendPacket(const std::vector<uint8_t>& data, std::shared_ptr<PeerWrapper> peer);
        bool sendSnapshotPacket(const SnapshotPacket& packet, std::shared_ptr<PeerWrapper> peer);
        bool pollEvent(ServerEvent& event);
    };

    class NetworkClient
    {
    private:
        std::unique_ptr<ENetHost, decltype(&enet_host_destroy)> client;
        std::unique_ptr<PeerWrapper> serverPeer;
        uint16_t lastPacketId = 0;

    public:
        NetworkClient();
        ~NetworkClient();

        bool connectToServer(const std::string& host, uint16_t port);
        void disconnect();
        bool sendPacket(const std::vector<uint8_t>& data);
        bool sendInputPacket(const InputPacket& packet);
        bool pollEvent(ClientEvent& event);
    };
}  // namespace network

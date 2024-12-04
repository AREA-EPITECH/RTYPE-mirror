/*
** EPITECH PROJECT, 2024
** RTYPE-mirror
** File description:
** Server.hpp
*/

#pragma once

#include <enet/enet.h>
#include <memory>
#include <cstdint>
#include <unordered_map>
#include <vector>

#include "PeerWrapper.hpp"
#include "event/Event.hpp"
#include "packet/Packet.hpp"

namespace network
{
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
        bool sendLobbyPacket(const LobbySnapshotPacket& packet, std::shared_ptr<PeerWrapper> peer);
        bool pollEvent(ServerEvent& event);
    };
} // namespace network

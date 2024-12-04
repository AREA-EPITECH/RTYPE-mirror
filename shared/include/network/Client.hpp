/*
** EPITECH PROJECT, 2024
** RTYPE-mirror
** File description:
** Client.hpp
*/

#pragma once

#include <enet/enet.h>
#include <memory>
#include <cstdint>
#include <vector>

#include "PeerWrapper.hpp"
#include "event/Event.hpp"
#include "packet/Packet.hpp"

namespace network
{
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
        bool sendLobbyPacket(const LobbyActionPacket& packet);
        bool pollEvent(ClientEvent& event);
    };
} // namespace network

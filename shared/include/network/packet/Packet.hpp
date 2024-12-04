/*
** EPITECH PROJECT, 2024
** RTYPE-mirror
** File description:
** Packet.hpp
*/

#pragma once

#include <utility>
#include <any>

#include "ClientPacket.hpp"
#include "ServerPacket.hpp"

namespace network
{
    class Packet
    {
    public:
        static std::vector<uint8_t> serializeSnapshotPacket(const SnapshotPacket &packet);
        static SnapshotPacket deserializeSnapshotPacket(const std::vector<uint8_t> &data);

        static std::vector<uint8_t> serializeInputPacket(const InputPacket &packet);
        static InputPacket deserializeInputPacket(const std::vector<uint8_t> &data);

        static std::vector<uint8_t> serializeLobbyActionPacket(const LobbyActionPacket &packet);
        static LobbyActionPacket deserializeLobbyActionPacket(const std::vector<uint8_t> &data);

        static std::vector<uint8_t> serializeLobbySnapshotPacket(const LobbySnapshotPacket &packet);
        static LobbySnapshotPacket deserializeLobbySnapshotPacket(const std::vector<uint8_t> &data);

        static std::pair<PacketType, std::any> deserializePacket(const std::vector<uint8_t>& data);
    };
} // namespace network

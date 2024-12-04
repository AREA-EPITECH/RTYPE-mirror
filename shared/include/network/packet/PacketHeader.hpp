/*
** EPITECH PROJECT, 2024
** RTYPE-mirror
** File description:
** PacketHeader.hpp
*/

#pragma once

#include <cstdint>

namespace network
{
#pragma pack(push, 1)
    enum class PacketType : uint8_t {
        RawPacket = 0,
        SnapshotPacket,
        InputPacket,
        LobbyActionPacket,
        LobbySnapshotPacket,
        NoPacket
    };

    struct PacketHeader
    {
        uint16_t packetId; // Unique ID for the packet type
        uint32_t timestamp; // Timestamp for synchronization
        PacketType type;
    };
#pragma pack(pop)
} // namespace network

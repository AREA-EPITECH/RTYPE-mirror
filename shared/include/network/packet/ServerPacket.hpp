/*
** EPITECH PROJECT, 2024
** RTYPE-mirror
** File description:
** ServerPacket.hpp
*/

#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "PacketHeader.hpp"

namespace network
{
#pragma pack(push, 1)
    // Game protocol
    enum EntityType
    {
        Player = 1,
        Opponent = 2,
        Rocket = 3,
        ChargedRocket = 4,
    };

    struct EntityUpdate
    {
        uint16_t entityId; // Unique ID of the entity
        EntityType type; // Type (e.g., enemy type)
        float posX, posY; // Position of the entity
        float velocityX, velocityY; // Optional velocity
    };

    struct SnapshotPacket
    {
        PacketHeader header;
        uint16_t numEntities; // Number of entities in this update
        std::vector<EntityUpdate> entities; // Array of entity updates
    };

    // Lobby protocol
    enum LobbyGameState
    {
        Waiting = 1,
        Starting = 2,
        Playing = 3,
    };

    struct LobbyPlayer
    {
        uint32_t id;
        std::string name;
        uint16_t shipId;
        bool ready;
    };

    struct LobbySnapshotPacket
    {
        PacketHeader header;
        uint32_t roomId;
        std::vector<LobbyPlayer> players;
        LobbyGameState gameState;
    };
#pragma pack(pop)
} // namespace network

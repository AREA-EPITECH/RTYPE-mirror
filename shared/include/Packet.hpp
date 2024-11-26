/*
** EPITECH PROJECT, 2024
** RTYPE-mirror
** File description:
** Packet.hpp
*/

#pragma once

#include <cstdint>
#include <vector>

namespace network
{
    struct PacketHeader
    {
        uint16_t packetId; // Unique ID for the packet type
        uint32_t timestamp; // Timestamp for synchronization
    };

    struct EntityUpdate
    {
        uint16_t entityId; // Unique ID of the entity
        uint8_t type; // Type (e.g., enemy type)
        float posX, posY; // Position of the entity
        float velocityX, velocityY; // Optional velocity
    };

    struct SnapshotPacket
    {
        PacketHeader header;
        uint16_t numEntities; // Number of entities in this update
        EntityUpdate entities[]; // Array of entity updates
    };


    class Packet
    {
    public:
        static std::vector<uint8_t> createSnapshotPacket(struct SnapshotPacket& entity);

        static void parsePacket(const std::vector<uint8_t> &data);
    };
} // namespace network

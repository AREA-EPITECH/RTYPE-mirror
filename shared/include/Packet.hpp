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
    #pragma pack(push, 1)
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
        std::vector<EntityUpdate> entities; // Array of entity updates
    };

    struct InputPacket
    {
        PacketHeader header;
        char *data; // data string
    };
    #pragma pack(pop)

    class Packet
    {
    public:
        static std::vector<uint8_t> serializeSnapshotPacket(const SnapshotPacket& packet);
        static SnapshotPacket deserializeSnapshotPacket(const std::vector<uint8_t>& data);

        static std::vector<uint8_t> serializeInputPacket(const InputPacket& packet);
        static InputPacket deserializeInputPacket(const std::vector<uint8_t>& data);
    };
} // namespace network

/*
** EPITECH PROJECT, 2024
** RTYPE-mirror
** File description:
** Packet.hpp
*/

#pragma once

#include <cstdint>
#include <vector>
#include <string>

namespace network
{
#pragma pack(push, 1)
    struct PacketHeader
    {
        uint16_t packetId; // Unique ID for the packet type
        uint32_t timestamp; // Timestamp for synchronization
    };

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

    enum MoveDirection
    {
        UpDirection = 1,
        DownDirection = 2,
        LeftDirection = 3,
        RightDirection = 4,
        NoneDirection = 0
    };

    enum FireType
    {
        NormalFire = 1,
        ChargedFire = 2,
        NoneFire = 0
    };

    struct InputPacket
    {
        PacketHeader header;
        MoveDirection move;
        FireType fire;
    };

    enum LobbyActionType
    {
        CreateRoom = 1,
        JoinRoom = 2,
        LeaveRoom = 3,
        ChangeName = 4,
        ChangeShip = 5,
        ChangeReady = 6
    };

    struct LobbyActionPacket
    {
        PacketHeader header;
        LobbyActionType actionType;
        uint32_t roomId;
        std::string name;
        uint16_t shipId;
        bool readyState;
    };

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
        uint32_t roomId;
        std::vector<LobbyPlayer> players;
        LobbyGameState gameState;
    };
#pragma pack(pop)

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
    };
} // namespace network

/*
** EPITECH PROJECT, 2024
** RTYPE-mirror
** File description:
** Packet.cpp
*/

#include "Packet.hpp"
#include <cstring>

std::vector<uint8_t> network::Packet::serializeSnapshotPacket(const network::SnapshotPacket &packet)
{
    std::vector<uint8_t> buffer;

    // Serialize the header
    buffer.resize(sizeof(PacketHeader));
    std::memcpy(buffer.data(), &packet.header, sizeof(PacketHeader));

    // Serialize the number of entities
    uint16_t numEntities = packet.numEntities;
    buffer.resize(buffer.size() + sizeof(uint16_t));
    std::memcpy(buffer.data() + buffer.size() - sizeof(uint16_t), &numEntities, sizeof(uint16_t));

    // Serialize each EntityUpdate
    for (const auto &entity : packet.entities)
    {
        size_t entitySize = sizeof(EntityUpdate);
        buffer.resize(buffer.size() + entitySize);
        std::memcpy(buffer.data() + buffer.size() - entitySize, &entity, entitySize);
    }

    return buffer;
}

network::SnapshotPacket network::Packet::deserializeSnapshotPacket(const std::vector<uint8_t> &data)
{
    network::SnapshotPacket packet;
    size_t offset = 0;

    // Deserialize the header
    std::memcpy(&packet.header, data.data() + offset, sizeof(PacketHeader));
    offset += sizeof(PacketHeader);

    // Deserialize the number of entities
    uint16_t numEntities = 0;
    std::memcpy(&numEntities, data.data() + offset, sizeof(uint16_t));
    offset += sizeof(uint16_t);
    packet.numEntities = numEntities;

    // Deserialize the entities
    packet.entities.resize(numEntities);
    for (uint16_t i = 0; i < numEntities; ++i)
    {
        std::memcpy(&packet.entities[i], data.data() + offset, sizeof(EntityUpdate));
        offset += sizeof(EntityUpdate);
    }

    return packet;
}

std::vector<uint8_t> network::Packet::serializeInputPacket(const network::InputPacket &packet)
{
    std::vector<uint8_t> buffer;
    size_t offset = 0;

    // Serialize PacketHeader
    size_t headerSize = sizeof(packet.header);
    buffer.resize(headerSize);
    std::memcpy(buffer.data(), &packet.header, headerSize);
    offset += headerSize;

    // Serialize MoveDirection
    buffer.resize(buffer.size() + sizeof(packet.move));
    std::memcpy(buffer.data() + offset, &packet.move, sizeof(packet.move));
    offset += sizeof(packet.move);

    // Serialize FireType
    buffer.resize(buffer.size() + sizeof(packet.fire));
    std::memcpy(buffer.data() + offset, &packet.fire, sizeof(packet.move));

    return buffer;
}

network::InputPacket network::Packet::deserializeInputPacket(const std::vector<uint8_t> &data)
{
    InputPacket packet;
    size_t offset = 0;

    // Deserialize PacketHeader
    size_t headerSize = sizeof(packet.header);
    std::memcpy(&packet.header, data.data(), headerSize);
    offset += headerSize;

    // Deserialize MoveDirection
    std::memcpy(&packet.move, data.data() + offset, sizeof(packet.move));
    offset += sizeof(packet.move);

    // Deserialize FireType
    std::memcpy(&packet.fire, data.data() + offset, sizeof(packet.fire));
    offset += sizeof(packet.fire);

    return packet;
}

std::vector<uint8_t> network::Packet::serializeLobbyActionPacket(const network::LobbyActionPacket &packet)
{
    std::vector<uint8_t> buffer;
    size_t offset = 0;

    // Serialize PacketHeader
    size_t headerSize = sizeof(packet.header);
    buffer.resize(headerSize);
    std::memcpy(buffer.data(), &packet.header, headerSize);
    offset += headerSize;

    // Serialize LobbyActionType
    buffer.resize(buffer.size() + sizeof(packet.actionType));
    std::memcpy(buffer.data() + offset, &packet.actionType, sizeof(packet.actionType));
    offset += sizeof(packet.actionType);

    if (packet.actionType != CreateRoom) {
        buffer.resize(buffer.size() + sizeof(packet.roomId));
        std::memcpy(buffer.data() + offset, &packet.roomId, sizeof(packet.roomId));
        offset += sizeof(packet.roomId);
    }

    switch (packet.actionType)
    {
    case ChangeName: {
        uint32_t nameLength = static_cast<uint32_t>(packet.name.size());
        buffer.resize(buffer.size() + sizeof(nameLength) + nameLength);
        std::memcpy(buffer.data() + offset, &nameLength, sizeof(nameLength));
        offset += sizeof(nameLength);
        std::memcpy(buffer.data() + offset, packet.name.data(), nameLength);
        offset += nameLength;
        break;
    }
    case ChangeShip:
        buffer.resize(buffer.size() + sizeof(packet.shipId));
        std::memcpy(buffer.data() + offset, &packet.shipId, sizeof(packet.shipId));
        offset += sizeof(packet.shipId);
        break;

    case ChangeReady:
        buffer.resize(buffer.size() + sizeof(packet.readyState));
        std::memcpy(buffer.data() + offset, &packet.readyState, sizeof(packet.readyState));
        offset += sizeof(packet.readyState);
        break;
    default:
        break;
    }

    return buffer;
}

network::LobbyActionPacket network::Packet::deserializeLobbyActionPacket(const std::vector<uint8_t> &data)
{
    LobbyActionPacket packet;
    size_t offset = 0;

    // Deserialize PacketHeader
    size_t headerSize = sizeof(packet.header);
    std::memcpy(&packet.header, data.data(), headerSize);
    offset += headerSize;

    // Deserialize LobbyActionType
    std::memcpy(&packet.actionType, data.data() + offset, sizeof(packet.actionType));
    offset += sizeof(packet.actionType);

    // Deserialize RoomId
    if (packet.actionType != CreateRoom) {
        std::memcpy(&packet.roomId, data.data() + offset, sizeof(packet.roomId));
        offset += sizeof(packet.roomId);
    }

    switch (packet.actionType)
    {
    case ChangeName: {
        uint32_t nameLength;
        std::memcpy(&nameLength, data.data() + offset, sizeof(nameLength));
        offset += sizeof(nameLength);

        packet.name.resize(nameLength);
        std::memcpy(&packet.name[0], data.data() + offset, nameLength);
        offset += nameLength;
        break;
    }
    case ChangeShip:
        std::memcpy(&packet.shipId, data.data() + offset, sizeof(packet.shipId));
        offset += sizeof(packet.shipId);
        break;

    case ChangeReady:
        std::memcpy(&packet.readyState, data.data() + offset, sizeof(packet.readyState));
        offset += sizeof(packet.readyState);
        break;
    default:
        break;
    }

    return packet;
}

std::vector<uint8_t> network::Packet::serializeLobbySnapshotPacket(const network::LobbySnapshotPacket &packet)
{
    std::vector<uint8_t> buffer;
    size_t offset = 0;

    // Serialize roomId
    buffer.resize(sizeof(packet.roomId));
    std::memcpy(buffer.data(), &packet.roomId, sizeof(packet.roomId));
    offset += sizeof(packet.roomId);

    // Serialize gameState
    buffer.resize(buffer.size() + sizeof(packet.gameState));
    std::memcpy(buffer.data() + offset, &packet.gameState, sizeof(packet.gameState));
    offset += sizeof(packet.gameState);

    // Serialize number of players
    uint32_t numPlayers = static_cast<uint32_t>(packet.players.size());
    buffer.resize(buffer.size() + sizeof(numPlayers));
    std::memcpy(buffer.data() + offset, &numPlayers, sizeof(numPlayers));
    offset += sizeof(numPlayers);

    // Serialize each player
    for (const auto& player : packet.players) {
        // Serialize player id
        buffer.resize(buffer.size() + sizeof(player.id));
        std::memcpy(buffer.data() + offset, &player.id, sizeof(player.id));
        offset += sizeof(player.id);

        // Serialize player name
        uint32_t nameLength = static_cast<uint32_t>(player.name.size());
        buffer.resize(buffer.size() + sizeof(nameLength) + nameLength);
        std::memcpy(buffer.data() + offset, &nameLength, sizeof(nameLength));
        offset += sizeof(nameLength);
        std::memcpy(buffer.data() + offset, player.name.data(), nameLength);
        offset += nameLength;

        // Serialize shipId
        buffer.resize(buffer.size() + sizeof(player.shipId));
        std::memcpy(buffer.data() + offset, &player.shipId, sizeof(player.shipId));
        offset += sizeof(player.shipId);

        // Serialize ready state
        buffer.resize(buffer.size() + sizeof(player.ready));
        std::memcpy(buffer.data() + offset, &player.ready, sizeof(player.ready));
        offset += sizeof(player.ready);
    }

    return buffer;
}

network::LobbySnapshotPacket network::Packet::deserializeLobbySnapshotPacket(const std::vector<uint8_t> &data)
{
    LobbySnapshotPacket packet;
    size_t offset = 0;

    // Deserialize roomId
    std::memcpy(&packet.roomId, data.data() + offset, sizeof(packet.roomId));
    offset += sizeof(packet.roomId);

    // Deserialize gameState
    std::memcpy(&packet.gameState, data.data() + offset, sizeof(packet.gameState));
    offset += sizeof(packet.gameState);

    // Deserialize number of players
    uint32_t numPlayers;
    std::memcpy(&numPlayers, data.data() + offset, sizeof(numPlayers));
    offset += sizeof(numPlayers);

    // Deserialize each player
    for (uint32_t i = 0; i < numPlayers; ++i) {
        LobbyPlayer player;

        // Deserialize player id
        std::memcpy(&player.id, data.data() + offset, sizeof(player.id));
        offset += sizeof(player.id);

        // Deserialize player name
        uint32_t nameLength;
        std::memcpy(&nameLength, data.data() + offset, sizeof(nameLength));
        offset += sizeof(nameLength);

        player.name.resize(nameLength);
        std::memcpy(&player.name[0], data.data() + offset, nameLength);
        offset += nameLength;

        // Deserialize shipId
        std::memcpy(&player.shipId, data.data() + offset, sizeof(player.shipId));
        offset += sizeof(player.shipId);

        // Deserialize ready state
        std::memcpy(&player.ready, data.data() + offset, sizeof(player.ready));
        offset += sizeof(player.ready);

        packet.players.push_back(player);
    }

    return packet;
}

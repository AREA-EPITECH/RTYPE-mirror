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
    for (const auto& entity : packet.entities) {
        size_t entitySize = sizeof(EntityUpdate);
        buffer.resize(buffer.size() + entitySize);
        std::memcpy(buffer.data() + buffer.size() - entitySize, &entity, entitySize);
    }

    return buffer;
}

network::SnapshotPacket network::Packet::deserializeSnapshotPacket(const std::vector<uint8_t>& data)
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
    for (uint16_t i = 0; i < numEntities; ++i) {
        std::memcpy(&packet.entities[i], data.data() + offset, sizeof(EntityUpdate));
        offset += sizeof(EntityUpdate);
    }

    return packet;
}

std::vector<uint8_t> network::Packet::serializeInputPacket(const network::InputPacket &packet)
{
    std::vector<uint8_t> buffer;

    // Serialize PacketHeader
    size_t headerSize = sizeof(packet.header);
    buffer.resize(headerSize);
    std::memcpy(buffer.data(), &packet.header, headerSize);

    // Serialize string length
    uint32_t dataLength = packet.data ? std::strlen(packet.data) : 0;
    buffer.resize(buffer.size() + sizeof(dataLength));
    std::memcpy(buffer.data() + headerSize, &dataLength, sizeof(dataLength));

    // Serialize string content
    if (dataLength > 0) {
        buffer.resize(buffer.size() + dataLength);
        std::memcpy(buffer.data() + headerSize + sizeof(dataLength), packet.data, dataLength);
    }

    return buffer;
}

network::InputPacket network::Packet::deserializeInputPacket(const std::vector<uint8_t>& data)
{
    InputPacket packet;

    size_t offset = 0;

    // Deserialize PacketHeader
    size_t headerSize = sizeof(packet.header);
    std::memcpy(&packet.header, data.data(), headerSize);
    offset += headerSize;

    // Deserialize string length
    uint32_t dataLength;
    std::memcpy(&dataLength, data.data() + offset, sizeof(dataLength));
    offset += sizeof(dataLength);

    // Deserialize string content
    if (dataLength > 0) {
        packet.data = new char[dataLength + 1]; // Allocate memory for string
        std::memcpy(packet.data, data.data() + offset, dataLength);
        packet.data[dataLength] = '\0'; // Null-terminate the string
    } else {
        packet.data = nullptr;
    }

    return packet;
}
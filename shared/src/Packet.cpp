/*
** EPITECH PROJECT, 2024
** RTYPE-mirror
** File description:
** Packet.cpp
*/

#include "Packet.hpp"

std::vector<uint8_t> network::Packet::createSnapshotPacket(network::SnapshotPacket &snapshot)
{
    std::vector<uint8_t> packet;
    packet.insert(packet.end(), reinterpret_cast<const uint8_t *>(&snapshot),
                  reinterpret_cast<const uint8_t *>(&snapshot) + sizeof(network::SnapshotPacket));
    return packet;
}

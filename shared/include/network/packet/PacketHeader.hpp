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

    /**
     * @enum PacketType
     * @brief Represents the type of a network packet.
     *
     * Enumerates the different categories of packets used in the RTYPE protocol,
     * helping to identify the packet's purpose and contents during transmission.
     */
    enum class PacketType : uint8_t
    {
        RawPacket = 0, ///< A raw, unprocessed packet.
        SnapshotPacket, ///< A snapshot of the game state.
        InputPacket, ///< A packet containing player input data.
        LobbyActionPacket, ///< A packet containing actions related to the lobby.
        LobbySnapshotPacket, ///< A snapshot of the current state of the lobby.
        NoPacket ///< Represents the absence of a valid packet.
    };

    /**
     * @struct PacketHeader
     * @brief Provides metadata for a network packet.
     *
     * The `PacketHeader` structure contains information used to manage and
     * identify packets during communication, such as a unique ID, a timestamp,
     * and the type of the packet.
     */
    struct PacketHeader
    {
        uint16_t packetId; ///< Unique identifier for the packet.
        uint32_t timestamp; ///< Timestamp for the packet (used for synchronization).
        PacketType type; ///< Type of the packet, as defined by the `PacketType` enum.
    };

#pragma pack(pop)
} // namespace network

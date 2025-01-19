/*
** EPITECH PROJECT, 2024
** RTYPE-mirror
** File description:
** Packet.hpp
*/

#pragma once

#include <any>
#include <utility>

#include "ClientPacket.hpp"
#include "ServerPacket.hpp"

namespace network
{
    /**
     * @class Packet
     * @brief Utility class for packet serialization and deserialization.
     *
     * This class provides static methods for converting packets between their in-memory
     * representations and binary formats. It supports multiple packet types used in
     * gameplay and lobby interactions.
     */
    class Packet
    {
    public:
        /**
         * @brief Serializes a `SnapshotPacket` to a binary format.
         * @param packet The `SnapshotPacket` to serialize.
         * @return A binary representation of the `SnapshotPacket`.
         */
        static std::vector<uint8_t> serializeSnapshotPacket(const struct SnapshotPacket &packet);

        /**
         * @brief Deserializes a binary format into a `SnapshotPacket`.
         * @param data The binary data to deserialize.
         * @return The deserialized `SnapshotPacket`.
         */
        static struct SnapshotPacket deserializeSnapshotPacket(const std::vector<uint8_t> &data);

        /**
         * @brief Serializes an `InputPacket` to a binary format.
         * @param packet The `InputPacket` to serialize.
         * @return A binary representation of the `InputPacket`.
         */
        static std::vector<uint8_t> serializeInputPacket(const struct InputPacket &packet);

        /**
         * @brief Deserializes a binary format into an `InputPacket`.
         * @param data The binary data to deserialize.
         * @return The deserialized `InputPacket`.
         */
        static struct InputPacket deserializeInputPacket(const std::vector<uint8_t> &data);

        /**
         * @brief Serializes a `LobbyActionPacket` to a binary format.
         * @param packet The `LobbyActionPacket` to serialize.
         * @return A binary representation of the `LobbyActionPacket`.
         */
        static std::vector<uint8_t> serializeLobbyActionPacket(const struct LobbyActionPacket &packet);

        /**
         * @brief Deserializes a binary format into a `LobbyActionPacket`.
         * @param data The binary data to deserialize.
         * @return The deserialized `LobbyActionPacket`.
         */
        static struct LobbyActionPacket deserializeLobbyActionPacket(const std::vector<uint8_t> &data);

        /**
         * @brief Serializes a `LobbySnapshotPacket` to a binary format.
         * @param packet The `LobbySnapshotPacket` to serialize.
         * @return A binary representation of the `LobbySnapshotPacket`.
         */
        static std::vector<uint8_t> serializeLobbySnapshotPacket(const struct LobbySnapshotPacket &packet);

        /**
         * @brief Deserializes a binary format into a `LobbySnapshotPacket`.
         * @param data The binary data to deserialize.
         * @return The deserialized `LobbySnapshotPacket`.
         */
        static struct LobbySnapshotPacket deserializeLobbySnapshotPacket(const std::vector<uint8_t> &data);

        /**
         * @brief Serializes a `ErrorPacket` to a binary format.
         * @param packet The `ErrorPacket` to serialize.
         * @return A binary representation of the `ErrorPacket`.
         */
        static std::vector<uint8_t> serializeErrorPacket(const struct ErrorPacket &packet);

        /**
         * @brief Deserializes a binary format into a `ErrorPacket`.
         * @param data The binary data to deserialize.
         * @return The deserialized `ErrorPacket`.
         */
        static struct ErrorPacket deserializeErrorPacket(const std::vector<uint8_t> &data);

        /**
         * @brief Deserializes a binary format into a recognized packet type.
         *
         * The function inspects the binary data to determine the packet type based on its
         * header, then uses the appropriate deserialization method to convert the binary
         * data into a structured packet. If the type is unrecognized or the data is incomplete,
         * the function returns the data as a `RawPacket`.
         *
         * @param data The binary buffer containing the packet data.
         * @return A pair consisting of:
         *   - The detected `PacketType` (or `RawPacket` if unrecognized).
         *   - The deserialized packet as an `std::any` (type depends on the packet).
         *
         * @note If the binary data does not contain a valid `PacketHeader`, it will be treated
         * as a `RawPacket`.
         */
        static std::pair<PacketType, std::any> deserializePacket(const std::vector<uint8_t> &data);
    };
} // namespace network

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

    /**
     * @enum EntityType
     * @brief Represents the type of an entity in the game.
     *
     * Each entity in the game is assigned a type to categorize it,
     * such as a player, opponent, or projectile.
     */
    enum EntityType
    {
        Player = 1, ///< Represents a player entity.
        Opponent = 2, ///< Represents an opponent entity.
        Rocket = 3, ///< Represents a basic rocket or projectile.
        ChargedRocket = 4, ///< Represents a charged rocket or enhanced projectile.
    };

    /**
     * @struct EntityUpdate
     * @brief Contains information about the state of an entity.
     *
     * Used in snapshot packets to describe the state of an entity,
     * including its position, velocity, and type.
     */
    struct EntityUpdate
    {
        uint32_t entityId; ///< Unique identifier for the entity.
        EntityType type; ///< The type of the entity (e.g., Player, Opponent).
        float posX, posY; ///< Position coordinates of the entity.
        uint32_t shipId; ///< Optional value to set shipId of the entity.
    };

    /**
     * @struct SnapshotPacket
     * @brief Represents a snapshot of the game state.
     *
     * This packet is sent to update clients with the latest state of
     * entities in the game, including their positions and types.
     */
    struct SnapshotPacket
    {
        PacketHeader header; ///< Header containing metadata about the packet.
        uint16_t numEntities; ///< The number of entities included in this snapshot.
        std::vector<EntityUpdate> entities; ///< List of entity updates in the snapshot.
    };

    /**
     * @enum LobbyGameState
     * @brief Represents the state of the game in a lobby context.
     *
     * Used to indicate the current phase of the game within a lobby,
     * such as waiting for players, starting, or actively playing.
     */
    enum LobbyGameState
    {
        Waiting = 1, ///< The lobby is waiting for players to join or ready up.
        Starting = 2, ///< The game is starting but not yet active.
        Playing = 3, ///< The game is currently in progress.
    };

    /**
     * @struct LobbyPlayer
     * @brief Represents a player's state in the lobby.
     *
     * Contains details about a player in the lobby, including their
     * unique ID, name, chosen ship, and readiness status.
     */
    struct LobbyPlayer
    {
        uint32_t id; ///< Unique identifier for the player.
        std::string name; ///< The player's display name.
        uint16_t shipId; ///< Identifier for the player's chosen ship.
        bool ready; ///< Indicates whether the player is ready.
    };

    /**
     * @struct LobbySnapshotPacket
     * @brief Represents a snapshot of the lobby state.
     *
     * This packet contains details about the current state of the lobby,
     * including player information, room ID, and game state.
     */
    struct LobbySnapshotPacket
    {
        PacketHeader header; ///< Header containing metadata about the packet.
        uint32_t roomId; ///< Unique identifier for the lobby room.
        std::vector<LobbyPlayer> players; ///< List of players currently in the lobby.
        LobbyGameState gameState; ///< Current state of the game in the lobby.
    };

#pragma pack(pop)
} // namespace network

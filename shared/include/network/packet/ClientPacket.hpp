/*
** EPITECH PROJECT, 2024
** RTYPE-mirror
** File description:
** ClientPacket.hpp
*/

#pragma once

#include <cstdint>
#include <string>

#include "PacketHeader.hpp"

namespace network
{
#pragma pack(push, 1)

    /**
     * @enum MoveDirection
     * @brief Represents the direction of a player's movement.
     *
     * Enumerates the possible movement directions that can be sent from the client.
     */
    enum MoveDirection
    {
        NoneDirection = 0, ///< No movement.
        UpDirection = 1, ///< Movement upwards.
        DownDirection = 2, ///< Movement downwards.
        LeftDirection = 3, ///< Movement to the left.
        RightDirection = 4 ///< Movement to the right.
    };

    /**
     * @enum FireType
     * @brief Represents the type of firing action performed by the player.
     *
     * Enumerates the types of firing actions a player can take.
     */
    enum FireType
    {
        NoneFire = 0, ///< No firing action.
        NormalFire = 1, ///< A normal fire action.
        ChargedFire = 2, ///< A charged fire action.
    };

    /**
     * @struct InputPacket
     * @brief Represents a player's input during gameplay.
     *
     * Contains the player's movement direction and firing action, along with
     * a header for synchronization and identification.
     */
    struct InputPacket
    {
        PacketHeader header; ///< Metadata for the packet.
        MoveDirection move; ///< Player's movement direction.
        FireType fire; ///< Player's firing action.
    };

    /**
     * @enum LobbyActionType
     * @brief Represents an action taken by a player in the game lobby.
     *
     * Enumerates the different actions a player can perform in the lobby,
     * such as creating or joining a room, or changing their settings.
     */
    enum LobbyActionType
    {
        CreateRoom = 1, ///< Create a new game room.
        JoinRoom = 2, ///< Join an existing game room.
        LeaveRoom = 3, ///< Leave the current game room.
        ChangeName = 4, ///< Change the player's display name.
        ChangeShip = 5, ///< Change the player's ship.
        ChangeReady = 6 ///< Toggle the player's ready state.
    };

    /**
     * @struct LobbyActionPacket
     * @brief Represents an action performed by a player in the game lobby.
     *
     * Contains details about the action, such as the action type, room information,
     * and player-specific attributes, along with a header for metadata.
     */
    struct LobbyActionPacket
    {
        PacketHeader header; ///< Metadata for the packet.
        LobbyActionType actionType; ///< Type of action performed in the lobby.
        uint32_t roomId; ///< ID of the room targeted by the action (if applicable).
        std::string name; ///< Player's display name (if applicable).
        uint16_t shipId; ///< ID of the ship selected by the player (if applicable).
        bool readyState; ///< Player's ready state in the lobby.
    };

#pragma pack(pop)
} // namespace network

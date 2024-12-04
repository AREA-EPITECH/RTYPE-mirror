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
    // Game protocol
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

    // Lobby protocol
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
#pragma pack(pop)
} // namespace network

/*
** EPITECH PROJECT, 2024
** RTYPE-mirror
** File description:
** ClientEvent.hpp
*/

#pragma once

#include <any>

#include "network/packet/Packet.hpp"
#include "network/packet/PacketHeader.hpp"

namespace network
{
    // Types of client event
    enum class ClientEventType {
        ServerDisconnect,
        DataReceive
    };

    // Client event struct
    struct ClientEvent {
        ClientEventType type;
        PacketType packetType;
        std::any data;
    };
} // namespace network

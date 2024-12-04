/*
** EPITECH PROJECT, 2024
** RTYPE-mirror
** File description:
** ServerEvent.hpp
*/

#pragma once

#include <memory>
#include <any>

#include "network/PeerWrapper.hpp"
#include "network/packet/PacketHeader.hpp"

namespace network
{
    // Types of server event
    enum class ServerEventType {
        ClientConnect,
        ClientDisconnect,
        DataReceive
    };

    // Server event struct
    struct ServerEvent {
        ServerEventType type;
        std::shared_ptr<PeerWrapper> peer;
        PacketType packetType;
        std::any data;
    };
} // namespace network

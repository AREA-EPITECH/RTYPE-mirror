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
    /**
     * @enum ClientEventType
     * @brief Enumerates the types of events a client can experience.
     *
     * - `ServerDisconnect`: Indicates the client has been disconnected from the server.
     * - `DataReceive`: Indicates that data has been received from the server.
     */
    enum class ClientEventType
    {
        ServerDisconnect, ///< Client disconnected from the server.
        DataReceive ///< Data received from the server.
    };

    /**
     * @struct ClientEvent
     * @brief Represents an event that occurs on the client side.
     *
     * The `ClientEvent` struct encapsulates the type of the event, the type of
     * packet associated with the event (if any), and the packet data itself.
     * The data is stored as an `std::any` to allow flexibility for different packet types.
     *
     * @var ClientEvent::type
     *     The type of event (`ClientEventType`).
     * @var ClientEvent::packetType
     *     The type of the packet received (`PacketType`), or `PacketType::NoPacket` if not applicable.
     * @var ClientEvent::data
     *     The event-specific data, represented as an `std::any`.
     */
    struct ClientEvent
    {
        ClientEventType type; ///< Type of the event.
        PacketType packetType; ///< Type of the associated packet, if applicable.
        std::any data; ///< The data associated with the event.
    };
} // namespace network

/*
** EPITECH PROJECT, 2024
** RTYPE-mirror
** File description:
** ServerEvent.hpp
*/

#pragma once

#include <any>
#include <memory>

#include "network/PeerWrapper.hpp"
#include "network/packet/PacketHeader.hpp"

namespace network
{
    /**
     * @enum ServerEventType
     * @brief Enumerates the types of events the server can experience.
     *
     * - `ClientConnect`: Indicates a client has connected to the server.
     * - `ClientDisconnect`: Indicates a client has disconnected from the server.
     * - `DataReceive`: Indicates that data has been received from a client.
     */
    enum class ServerEventType
    {
        ClientConnect, ///< A client connected to the server.
        ClientDisconnect, ///< A client disconnected from the server.
        DataReceive ///< Data received from a client.
    };

    /**
     * @struct ServerEvent
     * @brief Represents an event that occurs on the server side.
     *
     * The `ServerEvent` struct encapsulates the type of the event, the peer
     * associated with the event, the type of packet associated with the event
     * (if any), and the packet data itself. The data is stored as an `std::any`
     * to allow flexibility for different packet types.
     *
     * @var ServerEvent::type
     *     The type of event (`ServerEventType`).
     * @var ServerEvent::peer
     *     A shared pointer to the `PeerWrapper` representing the client involved in the event.
     * @var ServerEvent::packetType
     *     The type of the packet received (`PacketType`), or `PacketType::NoPacket` if not applicable.
     * @var ServerEvent::data
     *     The event-specific data, represented as an `std::any`.
     */
    struct ServerEvent
    {
        ServerEventType type; ///< Type of the event.
        std::shared_ptr<PeerWrapper> peer; ///< Client peer associated with the event.
        PacketType packetType; ///< Type of the associated packet, if applicable.
        std::any data; ///< The data associated with the event.
    };
} // namespace network

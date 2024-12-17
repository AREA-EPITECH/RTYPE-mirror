/*
** EPITECH PROJECT, 2024
** r-type
** File description:
** Room
*/

#include "Room.hpp"
#include <ClientData.hpp>
#include <Server.hpp>

namespace server
{
    Room::Room(const uint8_t id) : _id(id), _state(network::LobbyGameState::Waiting) {}

    bool Room::operator==(const Room &other) const { return this->_id == other._id; }

    /**
     * Convert std::vector<shared_ptr<PeerWrapper>> to std::vector<network::LobbyPlayer>
     * to be able to send paquet of type LobbySnapshotPaquet
     * @return {std::vector<network::LobbyPlayer>}
     */
    std::vector<network::LobbyPlayer> Room::toLobbyPlayers() const
    {
        std::vector<network::LobbyPlayer> lobby_players;
        for (auto client : this->_clients)
        {
            network::LobbyPlayer temp_player;
            ClientData data = client->getData<ClientData>();
            temp_player.id = data.getId();
            temp_player.name = data.getName();
            temp_player.ready = data.getReadyState();
            temp_player.shipId = data.getShipId();
            lobby_players.push_back(temp_player);
        }
        return lobby_players;
    }

    /**
     * Check if all clients are in ready state
     * @return {bool}
     */
    bool Room::getClientsReadiness() const
    {
        for (const auto &client : this->_clients)
        {
            if (!client->getData<ClientData>().getReadyState())
            {
                return false;
            }
        }
        return true;
    }

    /**
     * Send Packet to all clients of a room
     * Packets are either:
     * - LobbySnapshotPacket: rooms are in Waiting/Starting LobbyGameState
     * - SnapshotPacket: rooms are in Playing LobbyGameState
     * @param server - server reference
     */
    void Room::sendUpdateRoom(Server &server)
    {
        if (this->_state == network::LobbyGameState::Playing)
        {
            struct network::SnapshotPacket snapshot_packet;
            // TODO: populate snapshot_packet.entities + fill num_entities
            for (auto client : this->_clients)
            {
                server.getServer().sendSnapshotPacket(snapshot_packet, client);
            }
        }
        else
        {
            struct network::LobbySnapshotPacket lobby_snapshot_packet;
            lobby_snapshot_packet.roomId = this->_id;
            lobby_snapshot_packet.players = this->toLobbyPlayers();
            lobby_snapshot_packet.gameState = this->_state;
            for (auto client : this->_clients)
            {
                server.getServer().sendLobbyPacket(lobby_snapshot_packet, client);
            }
        }
    }

    /**
     * Getter client ID
     * @return {uint32_t}
     */
    uint32_t Room::getId() const { return this->_id; }

    /**
     * Getter for the size of the Room
     * @return {u_int8_t}
     */
    u_int8_t Room::getSize() const { return this->_clients.size(); }

    /**
     * Getter for room state
     * @return {LobbyGameState}
     */
    network::LobbyGameState Room::getState() const { return this->_state; }

    /**
     * Setter for room state
     * @param state
     */
    void Room::setState(network::LobbyGameState state) { this->_state = state; }
} // namespace server

/*
** EPITECH PROJECT, 2024
** r-type
** File description:
** Room
*/

#include "Room.hpp"
#include <ClientData.hpp>
#include <Server.hpp>
#include <GameData.hpp>

namespace server
{
    Room::Room(const uint32_t id) : _id(id), _state(network::LobbyGameState::Waiting) {
        _registry.register_component<std::shared_ptr<network::PeerWrapper>>();
        _registry.register_component<ClientData>();
        _registry.register_component<Pos>();
        _registry.register_component<Projectile>();
        _registry.register_component<MapComponent>();
        _registry.register_component<int>();
    }

    Room::~Room() {
        auto &clients = _registry.get_components<std::shared_ptr<network::PeerWrapper>>();
        for (int i = 0; i < clients.size(); i++) {
            if (clients[i].has_value()) {
                clients[i].value()->getData<ClientData>().unsetRoom();
                clients[i].value().reset();
                _registry.kill_entity(i);
            }
        }
    }

    void Room::addClient(std::shared_ptr<network::PeerWrapper> &peer) {
        const auto new_player = _registry.spawn_entity();
        auto &data = peer->getData<ClientData>();
        _registry.add_component<ClientData>(new_player, std::move(data));
        _registry.add_component<int>(new_player, 10);
        _registry.add_component<std::shared_ptr<network::PeerWrapper>>(new_player, std::move(peer));
    }

    std::shared_ptr<network::PeerWrapper> Room::getClient(uint32_t id) {
        auto &clients = _registry.get_components<std::shared_ptr<network::PeerWrapper>>();
        for (auto &client: clients) {
            if (client.has_value()) {
                if (client.value()->getData<ClientData>().getId() == id)
                    return client.value();
            }
        }
        return nullptr;
    }

    void Room::removeClient(uint32_t id) {
        auto &clients = _registry.get_components<std::shared_ptr<network::PeerWrapper>>();
        for (int i = 0; i < clients.size(); i++) {
            if (clients[i].has_value() && clients[i].value()->getData<ClientData>().getId() == id) {
                _registry.kill_entity(i);
            }
        }
    }

    bool Room::operator==(const Room &other) const { return this->_id == other._id; }

    std::vector<network::LobbyPlayer> Room::toLobbyPlayers() const
    {
        std::vector<network::LobbyPlayer> lobby_players;
        auto &clients = _registry.get_components<std::shared_ptr<network::PeerWrapper>>();
        for (int i = 0; i < clients.size(); i++) {
            if (clients[i].has_value()) {
                network::LobbyPlayer temp_player;
                auto &data = clients[i].value()->getData<ClientData>();
                temp_player.id = data.getId();
                temp_player.name = data.getName();
                temp_player.ready = data.getReadyState();
                temp_player.shipId = data.getShipId();
                lobby_players.push_back(temp_player);
            }
        }
        return lobby_players;
    }

    bool Room::getClientsReadiness() const
    {
        auto &clients = _registry.get_components<std::shared_ptr<network::PeerWrapper>>();
        for (int i = 0; i < clients.size(); i++) {
            if (clients[i].has_value()) {
                if (!clients[i].value()->getData<ClientData>().getReadyState()) {
                    return false;
                }
            }
        }
        return true;
    }

    void Room::sendUpdateRoom(Server &server)
    {
        if (this->_state == network::LobbyGameState::Playing)
        {
            // populate snapshot_packet with entities ONCE
            // match ECS entities with SnapshotPacket entities
            // send everything to all players
            struct network::SnapshotPacket snapshot_packet;
            auto &clients = _registry.get_components<std::shared_ptr<network::PeerWrapper>>();
            for (int i = 0; i < clients.size(); i++) {
                if (clients[i].has_value()) {
                    server.getServer().sendSnapshotPacket(snapshot_packet, clients[i].value());
                }
            }
        }
        else
        {
            static int count = 0;
            struct network::LobbySnapshotPacket lobby_snapshot_packet;
            lobby_snapshot_packet.roomId = this->_id;
            lobby_snapshot_packet.players = this->toLobbyPlayers();
            if (this->getClientsReadiness()) {
                this->_state = network::LobbyGameState::Starting;
                count++;
            } else {
                count = 0;
            }
            if (count == 100) {
                this->_state = network::LobbyGameState::Playing;
            }
            lobby_snapshot_packet.gameState = this->_state;
            auto &clients = _registry.get_components<std::shared_ptr<network::PeerWrapper>>();
            for (int i = 0; i < clients.size(); i++) {
                if (clients[i].has_value()) {
                    server.getServer().sendLobbyPacket(lobby_snapshot_packet, clients[i].value());
                }
            }
            if (this->_state == network::LobbyGameState::Playing) {
                server.changeRoomToPlaying(this->_id);
            }
        }
    }

    void Room::initPlaying() {
        auto &clients = _registry.get_components<std::shared_ptr<network::PeerWrapper>>();
        //auto map = _registry.spawn_entity();
        //_registry.add_component<MapComponent>(map, {"./server/levels/map1.json"});
        for (int i = 0; i < clients.size(); i++) {
            if (clients[i].has_value()) {
                _registry.add_component<Pos>(i, {0, 0});
                _registry.add_component<Projectile>(i, {0, 0, 0, 0, network::FireType::NoneFire});
            }
        }
    }

    uint32_t Room::getId() const { return this->_id; }

    void Room::setId(const uint32_t id) { this->_id = id; }

    network::LobbyGameState Room::getState() const { return this->_state; }

    void Room::setState(const network::LobbyGameState state) { this->_state = state; }
} // namespace server

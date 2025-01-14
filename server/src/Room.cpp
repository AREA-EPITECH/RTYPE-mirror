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
#include <spdlog/spdlog.h>

namespace server
{
    Room::Room(const uint32_t id) : _id(id), _state(network::LobbyGameState::Waiting) {
        _registry.register_component<std::shared_ptr<network::PeerWrapper>>();
        _registry.register_component<ClientData>();
        _registry.register_component<Pos>();
        _registry.register_component<Projectile>();
        _registry.register_component<Enemy>();
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

    /**
     * Convert Clients to a vector of LobbyPlayer in order to send them in a LobbySnapshotPaquet
     * @return {std::vector<network::LobbyPlayer>}
     */
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

    /**
     * Check if Clients all are ready in the room
     * @return {bool}
     */
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

    /**
     * Call updateProjectile every 100ms
     * @param elapsed_time time from runMainLoop
     */
    void Room::run(const long elapsed_time) {
        _accumulated_time += elapsed_time;
        if (_accumulated_time >= 100) {
            updateProjectile();
            _accumulated_time = 0;
        }
    }

    /**
     * Populate snapshot_packet with entities ONCE
     * Match ECS entities with SnapshotPacket entities
     * Send everything to all players
     * Send Paquet according to LobbyGameState
     * @param server : server reference
     */
    void Room::sendUpdateRoom(Server &server)
    {
        if (this->_state == network::LobbyGameState::Playing)
        {
            struct network::SnapshotPacket snapshot_packet;
            snapshot_packet.numEntities = 0;

            auto &clients = _registry.get_components<std::shared_ptr<network::PeerWrapper>>();
            auto &pos = _registry.get_components<Pos>();
            for (int i = 0; i < clients.size(); i++) {
                if (clients[i].has_value()) {
                    network::EntityUpdate entity_update;
                    entity_update.type = network::Player;
                    entity_update.entityId = clients[i].value()->getData<ClientData>().getId();
                    if (pos[i].has_value()) {
                        entity_update.posX = pos[i].value().x;
                        entity_update.posY = pos[i].value().y;
                    }
                    snapshot_packet.entities.push_back(entity_update);
                    snapshot_packet.numEntities += 1;
                }
            }

            auto &proj = _registry.get_components<Projectile>();
            for (int i = 0; i < proj.size(); i++) {
                if (proj[i].has_value()) {
                    network::EntityUpdate entity_update;
                    if (proj[i].value().type == network::NormalFire) {
                        entity_update.type = network::Rocket;
                    } else if (proj[i].value().type == network::ChargedFire) {
                        entity_update.type = network::ChargedRocket;
                    } else {
                        continue;
                    }
                    entity_update.entityId = i + 1;
                    entity_update.posX = proj[i].value().pos.x;
                    entity_update.posY = proj[i].value().pos.y;
                    entity_update.velocityX = proj[i].value().acceleration.x;
                    entity_update.velocityY = proj[i].value().acceleration.y;
                    snapshot_packet.entities.push_back(entity_update);
                    snapshot_packet.numEntities += 1;
                }
            }

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
                this->_state = network::LobbyGameState::Waiting;
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
                this->initPlaying();
            }
        }
    }

    void Room::initPlaying() {
        auto &clients = _registry.get_components<std::shared_ptr<network::PeerWrapper>>();
        for (int i = 0; i < clients.size(); i++) {
            if (clients[i].has_value()) {
                _registry.add_component<Pos>(i, {0, 0});
            }
        }

        MapComponent map("./server/levels/map1.json");
        for (int i = 0; i < map.enemies.size(); i++) {
            auto ennemy = _registry.spawn_entity();
            _registry.add_component<Enemy>(ennemy, {map.enemies[i].type, map.enemies[i].spawn_rate, map.enemies[i].score});
        }
    }

    void Room::addPos(const uint32_t client_id, network::MoveDirection type) {
        auto &clients = _registry.get_components<std::shared_ptr<network::PeerWrapper>>();
        for (int i = 0; i < clients.size(); i++) {
            if (clients[i].has_value() && clients[i].value()->getData<ClientData>().getId() == client_id) {
                auto &client_pos = _registry.get_components<Pos>();
                if (client_pos[i].has_value()) {
                    switch (type)
                    {
                        case network::MoveDirection::DownDirection: {
                            if (client_pos[i].value().y + 1 > MAXY_MAP) {
                                break;
                            }
                            client_pos[i].value().y += 1;
                        }
                            break;
                        case network::MoveDirection::UpDirection: {
                            if (client_pos[i].value().y - 1 < MINY_MAP) {
                                break;
                            }
                            client_pos[i].value().y -= 1;
                        }
                            break;
                        case network::MoveDirection::LeftDirection: {
                            if (client_pos[i].value().x - 1 < MINX_MAP) {
                                break;
                            }
                            client_pos[i].value().x -= 1;
                        }
                        break;
                        case network::MoveDirection::RightDirection: {
                            if (client_pos[i].value().x + 1 > MAXX_MAP) {
                                break;
                            }
                            client_pos[i].value().x += 1;
                        }
                            break;
                        case network::MoveDirection::NoneDirection:
                            default:
                                break;
                    }
                }
            }
        }
    }

    void Room::addProjectile(const uint32_t client_id, network::FireType type) {
        auto &clients = _registry.get_components<std::shared_ptr<network::PeerWrapper>>();
        Pos pos_client{};
        for (int i = 0; i < clients.size(); i++) {
            if (clients[i].has_value() && clients[i].value()->getData<ClientData>().getId() == client_id) {
                auto &client_pos = _registry.get_components<Pos>();
                if (client_pos[i].has_value()) {
                    pos_client.x = client_pos[i].value().x;
                    pos_client.y = client_pos[i].value().y;
                }
            }
        }
        Acceleration acc{};
        network::FireType proj_type;
        switch (type)
        {
            case network::FireType::ChargedFire:
                acc.x = 2;
                acc.y = 2;
                proj_type = network::FireType::ChargedFire;
                spdlog::info("Client {} shot a Charged Fire", client_id);
                break;
            case network::FireType::NormalFire:
                acc.x = 1;
                acc.y = 1;
                proj_type = network::FireType::NormalFire;
                spdlog::info("Client {} shot a Normal Fire", client_id);
            break;
            case network::FireType::NoneFire:
                default:
                proj_type = network::FireType::NoneFire;
                spdlog::info("Client {} didn't shot", client_id);
            break;
        }
        const auto new_proj = _registry.spawn_entity();
        _registry.add_component<Projectile>(new_proj, {pos_client.x, pos_client.y, acc.x, acc.y, proj_type});
    }

    void Room::updateProjectile() {
        auto &projectiles = _registry.get_components<Projectile>();
        for (int i = 0; i < projectiles.size(); i++) {
            if (projectiles[i].has_value() && projectiles[i].value().type != network::FireType::NoneFire) {
                if (projectiles[i].value().pos.x + 1 * projectiles[i].value().acceleration.x > ENDX_MAP) {
                    _registry.kill_entity(i);
                } else {
                    projectiles[i].value().pos.x += 1 * projectiles[i].value().acceleration.x;
                }
            }
        }
    }


    uint32_t Room::getId() const { return this->_id; }

    void Room::setId(const uint32_t id) { this->_id = id; }

    network::LobbyGameState Room::getState() const { return this->_state; }

    void Room::setState(const network::LobbyGameState state) { this->_state = state; }
} // namespace server

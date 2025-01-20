/*
** EPITECH PROJECT, 2024
** r-type
** File description:
** Room
*/

#include "Room.hpp"
#include <ClientData.hpp>
#include <GameData.hpp>
#include <Server.hpp>
#include <spdlog/spdlog.h>

#include "../../client/include/ecs/Display.hpp"

namespace server
{
    Room::Room(const uint32_t id) : _id(id), _state(network::LobbyGameState::Waiting)
    {
        _registry.register_component<std::shared_ptr<network::PeerWrapper>>();
        _registry.register_component<ClientData>();
        _registry.register_component<Pos>();
        _registry.register_component<Projectile>();
        _registry.register_component<Enemy>();
        _registry.register_component<Level>();
        _registry.register_component<int>();
        std::srand(std::time(nullptr));
    }

    Room::~Room()
    {
        this->kill_entities();
        spdlog::info("Room {} destroyed", _id);
    }

    void Room::kill_entities()
    {
        auto &enemies = _registry.get_components<Enemy>();
        for (int i = 0; i < enemies.size(); i++)
        {
            if (enemies[i].has_value())
            {
                _registry.kill_entity(i);
            }
        }

        auto &proj = _registry.get_components<Projectile>();
        for (int i = 0; i < proj.size(); i++)
        {
            if (proj[i].has_value())
            {
                _registry.kill_entity(i);
            }
        }

        kill_entities_with_component<Level>(_registry);
    }

    void Room::addClient(std::shared_ptr<network::PeerWrapper> &peer)
    {
        const auto new_player = _registry.spawn_entity();
        auto &data = peer->getData<ClientData>();
        _registry.add_component<ClientData>(new_player, std::move(data));
        _registry.add_component<int>(new_player, MAX_HEALTH);
        _registry.add_component<std::shared_ptr<network::PeerWrapper>>(new_player, std::move(peer));
    }

    std::shared_ptr<network::PeerWrapper> Room::getClient(uint32_t id)
    {
        auto &clients = _registry.get_components<std::shared_ptr<network::PeerWrapper>>();
        for (auto &client : clients)
        {
            if (client.has_value())
            {
                if (client.value()->getData<ClientData>().getId() == id)
                    return client.value();
            }
        }
        return nullptr;
    }

    Sparse_array<std::shared_ptr<network::PeerWrapper>> &Room::getClients()
    {
        return _registry.get_components<std::shared_ptr<network::PeerWrapper>>();
    }

    void Room::removeClient(uint32_t id)
    {
        auto &clients = _registry.get_components<std::shared_ptr<network::PeerWrapper>>();
        for (int i = 0; i < clients.size(); i++)
        {
            if (clients[i].has_value() && clients[i].value()->getData<ClientData>().getId() == id)
            {
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
        for (int i = 0; i < clients.size(); i++)
        {
            if (clients[i].has_value())
            {
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
        int i = 0;
        for (; i < clients.size(); i++)
        {
            if (clients[i].has_value())
            {
                if (!clients[i].value()->getData<ClientData>().getReadyState())
                {
                    return false;
                }
            }
        }
        if (i == 0) {
            return false;
        }
        return true;
    }

    /**
     * Call updateProjectile every 100ms
     * @param elapsed_time time from runMainLoop
     */
    void Room::run(long elapsed_time) {
        _accumulated_time += elapsed_time;
        _enemy_accumulated_time += elapsed_time;
        _projectile_accumulated_time += elapsed_time;

        this->updateEnemy();
        this->updateProjectile();
        this->checkCollisionProjectiles();
        this->checkCollisionVessels();

        if (_enemy_accumulated_time >= 100) {
            this->spawnEnemy();
            _enemy_accumulated_time = 0;
        }

        if (_projectile_accumulated_time >= 100) {
            this->spawnEnemyProjectile();
            _projectile_accumulated_time = 0;
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
            snapshot_packet.level = this->level;
            auto &levels = _registry.get_components<Level>();
            uint16_t score_to_achieve = 0;
            for (auto &level: levels) {
                if (level.has_value()) {
                    score_to_achieve = static_cast<uint16_t>(level.value().win_score);
                    break;
                }
            }
            snapshot_packet.maxScore = score_to_achieve;

            auto &pos = _registry.get_components<Pos>();

            auto &clients = _registry.get_components<std::shared_ptr<network::PeerWrapper>>();
            auto &lives = _registry.get_components<int>();
            for (int i = 0; i < clients.size(); i++) {
                if (clients[i].has_value() && clients[i].value()->getData<ClientData>().getAlive()) {
                    network::EntityUpdate entity_update;
                    entity_update.type = network::Player;
                    entity_update.entityId = clients[i].value()->getData<ClientData>().getId();
                    entity_update.shipId = clients[i].value()->getData<ClientData>().getShipId();
                    if (pos[i].has_value())
                    {
                        entity_update.posX = pos[i].value().x;
                        entity_update.posY = pos[i].value().y;
                    }
                    if (lives[i].value()) {
                        entity_update.health = lives[i].value();
                    }
                    entity_update.score = clients[i].value()->getData<ClientData>().getScore();
                    snapshot_packet.entities.push_back(entity_update);
                    snapshot_packet.numEntities += 1;
                }
            }

            auto &enemy = _registry.get_components<Enemy>();
            for (int i = 0; i < enemy.size(); i++)
            {
                if (enemy[i].has_value())
                {
                    network::EntityUpdate entity_update;
                    entity_update.type = network::EntityType::Opponent;
                    entity_update.entityId = i;
                    entity_update.shipId = enemy[i].value().type;
                    if (pos[i].has_value())
                    {
                        entity_update.posX = pos[i].value().x;
                        entity_update.posY = pos[i].value().y;
                    }
                    snapshot_packet.entities.push_back(entity_update);
                    snapshot_packet.numEntities += 1;
                }
            }

            auto &proj = _registry.get_components<Projectile>();
            for (int i = 0; i < proj.size(); i++)
            {
                if (proj[i].has_value())
                {
                    network::EntityUpdate entity_update;
                    if (proj[i].value().type == network::NormalFire)
                    {
                        entity_update.type = network::Rocket;
                    }
                    else if (proj[i].value().type == network::ChargedFire)
                    {
                        entity_update.type = network::ChargedRocket;
                    }
                    if (!proj[i].value()._from_player)
                    {
                        entity_update.type = network::OpponentRocket;
                    }
                    entity_update.entityId = i + 1;
                    entity_update.posX = proj[i].value().pos.x;
                    entity_update.posY = proj[i].value().pos.y;
                    snapshot_packet.entities.push_back(entity_update);
                    snapshot_packet.numEntities += 1;
                }
            }

            for (int i = 0; i < clients.size(); i++)
            {
                if (clients[i].has_value())
                {
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
            if (this->getClientsReadiness())
            {
                this->_state = network::LobbyGameState::Starting;
                count++;
            }
            else
            {
                this->_state = network::LobbyGameState::Waiting;
                count = 0;
            }
            if (count == 80)
            {
                this->_state = network::LobbyGameState::Playing;
            }
            lobby_snapshot_packet.gameState = this->_state;
            auto &clients = _registry.get_components<std::shared_ptr<network::PeerWrapper>>();
            for (int i = 0; i < clients.size(); i++)
            {
                if (clients[i].has_value())
                {
                    server.getServer().sendLobbyPacket(lobby_snapshot_packet, clients[i].value());
                }
            }
            if (this->_state == network::LobbyGameState::Playing)
            {
                server.changeRoomToPlaying(this->_id);
                this->initPlaying();
            }
        }
    }

    bool Room::initPlaying()
    {
        std::string level_map = fmt::format("./server/levels/map{}.json", level);
        if (!std::filesystem::exists(level_map)) {
            return false;
        }
        const MapComponent map(level_map);
        const auto level = _registry.spawn_entity();
        _registry.add_component<Level>(level, {map.level.name, map.level.win_score});
        auto &ecs_level = _registry.get_components<Level>();
        for (auto &ecs_lvl : ecs_level)
        {
            if (ecs_lvl.has_value())
            {
                spdlog::info("Level \"{}\" loaded. Score for win condition: {}", ecs_lvl.value().name,
                             ecs_lvl.value().win_score);
            }
        }
        for (int i = 0; i < map.enemies.size(); i++)
        {
            _enemies.emplace_back(map.enemies[i]);
        }
        auto &clients = _registry.get_components<std::shared_ptr<network::PeerWrapper>>();
        for (int i = 0; i < clients.size(); i++)
        {
            if (clients[i].has_value())
            {
                _registry.add_component<Pos>(i, {0, 0});
            }
        }
        return true;
    }

    void Room::addPos(const uint32_t client_id, network::MoveDirection type)
    {
        auto &clients = _registry.get_components<std::shared_ptr<network::PeerWrapper>>();
        auto &client_pos = _registry.get_components<Pos>();
        for (int i = 0; i < clients.size(); i++)
        {
            if (clients[i].has_value() && clients[i].value()->getData<ClientData>().getId() == client_id)
            {
                if (client_pos[i].has_value())
                {
                    switch (type)
                    {
                    case network::MoveDirection::DownDirection:
                        {
                            if (client_pos[i].value().y + 1 > MAXY_MAP)
                            {
                                break;
                            }
                            client_pos[i].value().y += 1;
                        }
                        break;
                    case network::MoveDirection::UpDirection:
                        {
                            if (client_pos[i].value().y - 1 < MINY_MAP)
                            {
                                break;
                            }
                            client_pos[i].value().y -= 1;
                        }
                        break;
                    case network::MoveDirection::LeftDirection:
                        {
                            if (client_pos[i].value().x - 1 < MINX_MAP + 200)
                            {
                                break;
                            }
                            client_pos[i].value().x -= 1;
                        }
                        break;
                    case network::MoveDirection::RightDirection:
                        {
                            if (client_pos[i].value().x + 1 > MAXX_MAP)
                            {
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

    void Room::addProjectile(const uint32_t client_id, network::FireType type)
    {
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
            acc.x = 10;
            acc.y = 10;
            proj_type = network::FireType::NormalFire;
            spdlog::info("Client {} shot a Normal Fire", client_id);
            break;
        case network::FireType::NoneFire:
        default:
            return;
        }

        auto &clients = _registry.get_components<std::shared_ptr<network::PeerWrapper>>();
        Pos pos_client{};
        entity_t client_entity;
        for (int i = 0; i < clients.size(); i++)
        {
            if (clients[i].has_value() && clients[i].value()->getData<ClientData>().getId() == client_id)
            {
                auto &client_pos = _registry.get_components<Pos>();
                if (client_pos[i].has_value())
                {
                    pos_client.x = client_pos[i].value().x;
                    pos_client.y = client_pos[i].value().y;
                    client_entity = i;
                }
            }
        }

        const auto new_proj = _registry.spawn_entity();
        _registry.add_component<Projectile>(new_proj, {pos_client.x, pos_client.y, acc.x, acc.y, proj_type, true, client_entity});
    }

    void Room::updateProjectile()
    {
        auto &projectiles = _registry.get_components<Projectile>();
        for (int i = 0; i < projectiles.size(); i++)
        {
            if (projectiles[i].has_value() && projectiles[i].value().type != network::FireType::NoneFire)
            {
                if (projectiles[i].value()._from_player)
                {
                    if (projectiles[i].value().pos.x >= ENDX_MAP)
                    {
                        _registry.kill_entity(i);
                    }
                    else
                    {
                        projectiles[i].value().pos.x += 1 * projectiles[i].value().acceleration.x;
                    }
                }
                else
                {
                    if (projectiles[i].value().pos.x <= MINX_MAP)
                    {
                        _registry.kill_entity(i);
                    }
                    else
                    {
                        projectiles[i].value().pos.x -= 1 * projectiles[i].value().acceleration.x;
                    }
                }
            }
        }
    }

    void Room::spawnEnemy()
    {
        for (auto &enemy : _enemies)
        {
            enemy.spawn_clock += 100;
            if (enemy.spawn_clock >= enemy.spawn_rate * 1000)
            {
                const auto new_enemy = _registry.spawn_entity();
                int random_y;
                if (enemy.type == Easy)
                {
                    random_y = std::rand() % (MAXY_MAP + 1);
                }
                else
                {
                    random_y = std::rand() % (MAXY_MAP - 400 + 1) + 200;
                }
                if (random_y + enemy.hitbox.y >= MAXY_MAP)
                {
                    random_y -= enemy.hitbox.y;
                }
                if (random_y - enemy.hitbox.y <= MINY_MAP)
                {
                    random_y += enemy.hitbox.y;
                }
                enemy.init_pos.y = random_y;
                enemy.init_pos.x = ENDX_MAP;
                _registry.add_component<Enemy>(new_enemy,
                        {enemy.type, enemy.spawn_rate, enemy.shot_rate, enemy.spawn_clock, enemy.shot_clock, enemy.score, enemy.hitbox,
                        enemy.acc, enemy.init_pos, enemy.moveFunction});
                _registry.add_component<Pos>(new_enemy, {ENDX_MAP, random_y});
                this->addProjectileEnemy(enemy, enemy.init_pos);
                spdlog::info("Spawned enemy of type {} with spawn rate {} at [{};{}]", static_cast<int>(enemy.type),
                             enemy.spawn_rate, enemy.init_pos.x, random_y);
                enemy.spawn_clock = 0;
            }
        }
    }

    void Room::addProjectileEnemy(const Enemy &enemy, const Pos &pos)
    {
        const auto new_proj = _registry.spawn_entity();
        Acceleration acc{};
        switch (enemy.type)
        {
        case Easy:
            acc.x = 7;
            acc.y = 7;
            break;
        case Medium:
            acc.x = 8;
            acc.y = 8;
            break;
        case Hard:
            acc.x = 10;
            acc.y = 10;
            break;
        default:
            break;
        }
        _registry.add_component<Projectile>(new_proj, {pos, acc, network::FireType::NormalFire, false, 0});
    }

    void Room::spawnEnemyProjectile() {
        auto &enemies = _registry.get_components<Enemy>();
        auto &pos = _registry.get_components<Pos>();
        for (int i = 0; i < enemies.size(); i++) {
            if (enemies[i].has_value() && pos[i].has_value()) {
                enemies[i].value().shot_clock += 100;
                if (enemies[i].value().shot_clock >= enemies[i].value().shot_rate * 1000) {
                    this->addProjectileEnemy(enemies[i].value(), pos[i].value());
                    enemies[i].value().shot_clock = 0;
                }
            }
        }
    }

    void Room::updateEnemy()
    {
        auto &enemies = _registry.get_components<Enemy>();
        auto &pos = _registry.get_components<Pos>();
        for (int i = 0; i < enemies.size(); i++)
        {
            if (enemies[i].has_value())
            {
                if (pos[i].has_value())
                {
                    pos[i].value().x -= 1 * enemies[i].value().acc.x;
                    pos[i].value().y =
                        (enemies[i].value().init_pos.y + enemies[i].value().moveFunction(pos[i].value().x));
                    if (pos[i].value().x < MINX_MAP)
                    {
                        _registry.kill_entity(i);
                    }
                }
            }
        }
    }

    void Room::checkCollisionVessels() {
        auto &clients = _registry.get_components<std::shared_ptr<network::PeerWrapper>>();
        auto &enemies = _registry.get_components<Enemy>();
        auto &pos = _registry.get_components<Pos>();
        auto &lives = _registry.get_components<int>();
        for (int i = 0; i < clients.size(); i++) {
            if (clients[i].has_value() && clients[i].value()->getData<ClientData>().getAlive() && pos[i].has_value()) {
                for (int j = 0; j < enemies.size() && clients[i].has_value(); j++) {
                    if (enemies[j].has_value() && pos[j].has_value()) {
                        auto &data = clients[i].value()->getData<ClientData>();
                        Pos hitbox = data.getHitbox();
                        if (pos[i].value().x < pos[j].value().x + enemies[j].value().hitbox.x &&
                            pos[i].value().x + data.getHitbox().x > pos[j].value().x - enemies[j].value().hitbox.x / 2 &&
                            pos[i].value().y - hitbox.y < pos[j].value().y &&
                            pos[i].value().y > pos[j].value().y - enemies[j].value().hitbox.y) {
                            spdlog::info("Enemy {} is dead by vessel", j);
                            if (lives[i].has_value()) {
                                lives[i].value() -= 1;
                                if (lives[i].value() == 0) {
                                    data.setAlive();
                                    spdlog::info("Client {} is dead by vessel", data.getId());
                                }
                            }
                            clients[i].value()->getData<ClientData>().setScore(enemies[j].value().score);
                            _registry.kill_entity(j);
                        }
                    }
                }
            }

        }
    }

    void Room::checkCollisionProjectiles() {
        auto &proj = _registry.get_components<Projectile>();
        auto &clients = _registry.get_components<std::shared_ptr<network::PeerWrapper>>();
        auto &enemies = _registry.get_components<Enemy>();
        auto &pos = _registry.get_components<Pos>();
        auto &lives = _registry.get_components<int>();
        for (int i = 0; i < proj.size(); i++) {
            if (proj[i].has_value()) {
                if (proj[i].value()._from_player) {
                    for (int j = 0; j < enemies.size() && proj[i].has_value(); j++) {
                        if (enemies[j].has_value() && pos[j].has_value()) {
                            if (proj[i].value().pos.x >= pos[j].value().x - enemies[j].value().hitbox.x &&
                                proj[i].value().pos.x <= pos[j].value().x + enemies[j].value().hitbox.x &&
                                proj[i].value().pos.y >= pos[j].value().y - enemies[j].value().hitbox.y &&
                                proj[i].value().pos.y <= pos[j].value().y + enemies[j].value().hitbox.y) {
                                spdlog::info("Enemy {} is dead by shot", j);
                                if (clients[proj[i].value().player_id].has_value()) {
                                    clients[proj[i].value().player_id].value()->getData<ClientData>().setScore(enemies[j].value().score);
                                }
                                _registry.kill_entity(i);
                                _registry.kill_entity(j);
                            }
                        }
                    }
                } else {
                    for (int j = 0; j < clients.size() && proj[i].has_value(); j++) {
                        if (clients[j].has_value() && clients[j].value()->getData<ClientData>().getAlive() && pos[j].has_value()) {
                            auto &data = clients[j].value()->getData<ClientData>();
                            Pos hitbox = data.getHitbox();
                            if (proj[i].value().pos.x >= pos[j].value().x - hitbox.x / 2 &&
                                proj[i].value().pos.x <= pos[j].value().x + hitbox.x / 2 &&
                                proj[i].value().pos.y >= pos[j].value().y - hitbox.y  &&
                                proj[i].value().pos.y <= pos[j].value().y) {
                                _registry.kill_entity(i);
                                spdlog::info("Client {} is shot", data.getId());
                                if (lives[j].has_value()) {
                                    lives[j].value() -= 1;
                                    if (lives[j].value() == 0) {
                                        data.setAlive();
                                        spdlog::info("Client {} is dead by shot", data.getId());
                                    }
                                }
                            }
                        }
                    }
                }

            }
        }
    }

    bool Room::checkWin() {
        auto &clients = _registry.get_components<std::shared_ptr<network::PeerWrapper>>();
        auto &levels = _registry.get_components<Level>();
        int score_to_achieve = 0;
        for (auto &level: levels) {
            if (level.has_value()) {
                score_to_achieve = level.value().win_score;
                break;
            }
        }
        for (int i = 0; i < clients.size(); i++) {
            if (clients[i].has_value() && clients[i].value()->getData<ClientData>().getScore() >= score_to_achieve) {
                return true;
            }
        }
        return false;
    }

    bool Room::checkLose() {
        auto &clients = _registry.get_components<std::shared_ptr<network::PeerWrapper>>();
        for (int i = 0; i < clients.size(); i++) {
            if (clients[i].has_value() && clients[i].value()->getData<ClientData>().getAlive()) {
                return false;
            }
        }
        return true;
    }

    uint32_t Room::getId() const { return this->_id; }

    void Room::setId(const uint32_t id) { this->_id = id; }

    int Room::getLevel() const { return this->level; }

    void Room::setLevel(const int level, server::Server &server) {
        this->level = level;
        this->sendUpdateRoom(server);
        this->setState(network::LobbyGameState::Waiting);
        this->kill_entities();
        auto &clients = _registry.get_components<std::shared_ptr<network::PeerWrapper>>();
        auto &life = _registry.get_components<int>();
        for (int i = 0; i < clients.size(); i++)
        {
            if (clients[i].has_value())
            {
                auto &data = clients[i].value()->getData<ClientData>();
                data.resetScore();
                spdlog::info("Reset score");
                if (data.getReadyState()) {
                    data.setReadyState();
                }
            }
            if (life[i].has_value()) {
                life[i].value() = MAX_HEALTH;
            }
        }
    }

    bool Room::isClientinsideRoom()
    {
        auto &clients = _registry.get_components<std::shared_ptr<network::PeerWrapper>>();
        for (int i = 0; i < clients.size(); i++)
        {
            if (clients[i].has_value())
            {
                return true;
            }
        }
        return false;
    }

    network::LobbyGameState Room::getState() const { return this->_state; }

    void Room::setState(const network::LobbyGameState state) { this->_state = state; }


    uint32_t Room::getNumberClients() const {
        auto &clients = _registry.get_components<std::shared_ptr<network::PeerWrapper>>();
        uint32_t count = 0;
        for (int i = 0; i < clients.size(); i++) {
            if (clients[i].has_value()) {
                count++;
            }
        }
        return count;
    }


    Registry &Room::getRegistry() {
        return this->_registry;
    }

    MapComponent::MapComponent(const std::string &filePath)
    {
        std::ifstream file(filePath);
        if (!file.is_open())
        {
            throw std::runtime_error("Unable to open file: " + filePath);
        }

        nlohmann::json jsonData;
        file >> jsonData;

        level.name = jsonData.at("name").get<std::string>();
        level.win_score = jsonData.at("win_score").get<int>();

        for (const auto &enemyData : jsonData.at("enemies"))
        {
            Enemy enemy;
            enemy.type = enemyData.at("type").get<EnemyType>();
            enemy.spawn_rate = enemyData.at("spawn_rate").get<int>();
            enemy.score = enemyData.at("score").get<int>();
            enemy.acc.x = enemyData.at("acc").get<int>();
            enemy.acc.y = enemyData.at("acc").get<int>();
            enemy.shot_rate = enemyData.at("shot_rate").get<int>();
            enemy.spawn_clock = 0;
            enemy.shot_clock = 0;
            switch (enemy.type)
            {
            case Easy:
                enemy.hitbox.x = 75;
                enemy.hitbox.y = 35;
                enemy.moveFunction = [](int x) { return 0; };
                break;
            case Medium:
                enemy.hitbox.x = 90;
                enemy.hitbox.y = 30;
                enemy.moveFunction = [](int x) { return (MAXX_MAP / 2.5) * sin(x * 0.01); };
                break;
            case Hard:
                enemy.hitbox.x = 42;
                enemy.hitbox.y = 140;
                enemy.moveFunction = [](int x)
                {
                    double T = 800;
                    double A = 200;
                    double mod = std::fmod(x, T);
                    return (2 * A / T) * std::abs(mod - T / 2) - A;
                };
                break;
            default:
                break;
            }
            enemies.emplace_back(enemy);
        }
    }
} // namespace server

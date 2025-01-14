/*
** EPITECH PROJECT, 2024
** r-type
** File description:
** Room
*/

#pragma once
#include <cstdint>
#include <memory>
#include <Server.hpp>
#include <enet/enet.h>
#include <vector>
#include <network/PeerWrapper.hpp>
#include <network/packet/ServerPacket.hpp>
#include <network/packet/ClientPacket.hpp>
#include <Registry.hpp>
#include <nlohmann/json.hpp>
#include <fstream>

namespace server {
#define MINX_MAP 0
#define MAXX_MAP 350
#define ENDX_MAP 500
#define MINY_MAP 0
#define MAXY_MAP 332
    class Server;
    class Room {
        uint32_t _id;
        network::LobbyGameState _state;
        Registry _registry;
    public:
        explicit Room(uint32_t id);
        ~Room();
        bool operator==(const Room& other) const;

        std::vector<network::LobbyPlayer> toLobbyPlayers() const;
        void sendUpdateRoom(Server &server);
        void initPlaying();

        void addClient(std::shared_ptr<network::PeerWrapper> &peer);
        std::shared_ptr<network::PeerWrapper> getClient(uint32_t id);
        void removeClient(uint32_t id);

        void addPos(uint32_t client_id, network::MoveDirection type);
        void addProjectile(uint32_t client_id, network::FireType type);

        void updateProjectile();

        bool getClientsReadiness() const;
        uint32_t getId() const;
        void setId(uint32_t id);
        network::LobbyGameState getState() const;
        void setState(network::LobbyGameState state);
    };

    struct Enemy {
        std::string type;
        int spawn_rate;
        int score;
    };

    class MapComponent {
    public:
        std::string name;
        int win_score;
        std::vector<Enemy> enemies;

        MapComponent(const std::string &filePath) {
            std::ifstream file(filePath);
            if (!file.is_open()) {
                throw std::runtime_error("Unable to open file: " + filePath);
            }

            nlohmann::json jsonData;
            file >> jsonData;

            name = jsonData.at("name").get<std::string>();
            win_score = jsonData.at("win_score").get<int>();

            for (const auto &enemyData : jsonData.at("enemies")) {
                Enemy enemy;
                enemy.type = enemyData.at("type").get<std::string>();
                enemy.spawn_rate = enemyData.at("spawn_rate").get<int>();
                enemy.score = enemyData.at("score").get<int>();
                enemies.push_back(enemy);
            }
        }
    };
} // namespace server
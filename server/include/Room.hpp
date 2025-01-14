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
#include <cstdlib>
#include <GameData.hpp>

namespace server {
#define MINX_MAP 0
#define MAXX_MAP 350
#define ENDX_MAP 500
#define MINY_MAP 0
#define MAXY_MAP 332
#define RAY 100
    class Server;
    struct Enemy;
    class Room {
        uint32_t _id;
        network::LobbyGameState _state;
        Registry _registry;
        long _accumulated_time = 0;
        long _enemy_accumulated_time = 0;
        std::vector<Enemy> _enemies;
    public:
        explicit Room(uint32_t id);
        ~Room();
        bool operator==(const Room& other) const;

        void run(long elapsed_time);

        std::vector<network::LobbyPlayer> toLobbyPlayers() const;
        void sendUpdateRoom(Server &server);
        void initPlaying();

        void addClient(std::shared_ptr<network::PeerWrapper> &peer);
        std::shared_ptr<network::PeerWrapper> getClient(uint32_t id);
        void removeClient(uint32_t id);

        void addPos(uint32_t client_id, network::MoveDirection type);
        void addProjectile(uint32_t client_id, network::FireType type);

        void spawnEnnemy();
        void updateEnnemy();
        void updateProjectile();

        bool getClientsReadiness() const;
        uint32_t getId() const;
        void setId(uint32_t id);
        network::LobbyGameState getState() const;
        void setState(network::LobbyGameState state);
    };

    enum EnemyType
    {
        Easy = 0,
        Medium = 1,
        Hard = 2,
    };

    struct Enemy {
        EnemyType type;
        int spawn_rate;
        long clock;
        int score;
        Pos hitbox;
        Pos init_pos;
        std::function<int (int)> moveFunction;
    };

    struct Level {
        std::string name;
        int win_score;
    };

    class MapComponent {
    public:
        Level level;
        std::vector<Enemy> enemies;
        MapComponent(const std::string &filePath);
    };
} // namespace server
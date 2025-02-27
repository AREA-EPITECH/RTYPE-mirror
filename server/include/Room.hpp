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
#define MINX_MAP -250
#define MAXX_MAP 350
#define ENDX_MAP 950
#define MINY_MAP 0
#define MAXY_MAP 332
#define MAX_HEALTH 10
#define MAX_CLIENTS 4
    class Server;
    struct Enemy;
    class Room {
        uint32_t _id;
        network::LobbyGameState _state;
        Registry _registry;
        long _accumulated_time = 0;
        long _enemy_accumulated_time = 0;
        long _projectile_accumulated_time = 0;
        std::vector<Enemy> _enemies;
        int level = 1;
        bool _boss_spawned = false;
    public:
        explicit Room(uint32_t id);
        ~Room();
        bool operator==(const Room& other) const;

        void kill_entities();

        void run(long elapsed_time);

        std::vector<network::LobbyPlayer> toLobbyPlayers() const;
        void sendUpdateRoom(Server &server);
        bool initPlaying();

        void addClient(std::shared_ptr<network::PeerWrapper> &peer);
        std::shared_ptr<network::PeerWrapper> getClient(uint32_t id);
        Sparse_array<std::shared_ptr<network::PeerWrapper>> &getClients();
        void removeClient(uint32_t id);

        void addPos(uint32_t client_id, network::MoveDirection type);
        void addProjectile(uint32_t client_id, network::FireType type);

        void addProjectileEnemy(const Enemy &enemy, const Pos &pos);
        void spawnEnemy();
        void spawnEnemyProjectile();
        void updateEnemy();
        void updateProjectile();

        void checkCollisionVessels();
        void checkCollisionProjectiles();

        bool checkWin();
        bool checkLose();

        bool getClientsReadiness() const;
        uint32_t getId() const;
        void setId(uint32_t id);
        int getLevel() const;
        void setLevel(int level, server::Server &server);
        bool isClientinsideRoom();
        network::LobbyGameState getState() const;
        void setState(network::LobbyGameState state);
        uint32_t getNumberClients() const;

        Registry &getRegistry();
    };

    enum EnemyType
    {
        Easy = 0,
        Medium = 1,
        Hard = 2,
        Boss = 3,
    };

    struct Enemy {
        EnemyType type;
        int spawn_rate;
        int shot_rate;
        long spawn_clock;
        long shot_clock;
        int score;
        Pos hitbox;
        Acceleration acc;
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
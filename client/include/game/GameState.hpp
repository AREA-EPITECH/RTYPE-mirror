/*
** EPITECH PROJECT, 2024
** RTYPE-mirror
** File description:
** GameState.hpp
*/

#pragma once

#include <cstdint>
#include <string>
#include <tuple>
#include <vector>
#include <map>

#include "Registry.hpp"
#include "ecs/Systems.hpp"

namespace game
{
    class GameState {
    public:
        enum LobbyGameState {
            Waiting = network::LobbyGameState::Waiting,
            Starting = network::LobbyGameState::Starting,
            Playing = network::LobbyGameState::Playing,
            Menu = 4,
        };

        struct Player {
            uint32_t id;
            entity_t entity;
            std::string name;
            uint16_t ship_id = 0;
            bool is_ready = false;
            std::tuple<uint, uint> position;
            int score = 0;
        };

        std::map<uint32_t, entity_t> enemy_entities;

        GameState();
        GameState(const Player &player);
        ~GameState();

        auto getUser() -> Player;
        auto updateUser(const Player &player) -> void;

        auto getOtherPlayer() -> std::vector<Player>;
        auto updateOtherPlayer(const std::vector<Player> &players) -> void;

        auto getGameState() -> LobbyGameState;
        auto updateGameState(LobbyGameState state) -> void;

        auto getRoomId() -> uint32_t;
        auto setRoomId(uint32_t _roomId) -> void;

        auto getIdEntity() -> entity_t;
        auto setIdEntity(entity_t entity) -> void;

        auto getEnemyEntities() -> std::map<uint32_t, entity_t>;
        auto setEnemyEntities(const std::map<uint32_t, entity_t> &entities) -> void;

        auto getShowScore() -> bool;
        auto setShowScore(bool show) -> void;

        auto getPlayerNameEntities() -> std::vector<entity_t>;
        auto addPlayerNameEntities(entity_t entity) -> void;
        auto removePlayerNameEntities(entity_t entity) -> void;

    private:
        Player user;
        std::vector<Player> other_players;
        LobbyGameState game_state = LobbyGameState::Waiting;
        uint32_t roomId;
        bool show_score = false;

        entity_t textIdEntity;
        std::vector<entity_t> textPlayerNameEntities;
    };
} // namespace game

std::optional<std::reference_wrapper<game::GameState>> getGameState(Registry &ecs);

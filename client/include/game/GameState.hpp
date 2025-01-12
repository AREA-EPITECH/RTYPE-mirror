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

#include "Registry.hpp"
#include "ecs/Systems.hpp"

namespace game
{
    class GameState {
    public:
        struct Player {
            uint32_t id;
            uint16_t health = MAX_HEALTH;
            std::string name;
            uint16_t ship_id = 0;
            bool is_ready = false;
            std::tuple<uint, uint> position;
        };

        GameState();
        GameState(const Player &player);
        ~GameState();

        auto getUser() -> Player;
        auto updateUser(const Player &player) -> void;

        auto getOtherPlayer() -> std::vector<Player>;
        auto updateOtherPlayer(const std::vector<Player> &players) -> void;

        auto getGameState() -> network::LobbyGameState;
        auto updateGameState(network::LobbyGameState state) -> void;

        auto getRoomId() -> uint32_t;
        auto setRoomId(uint32_t _roomId) -> void;

        auto getIdEntity() -> entity_t;
        auto setIdEntity(entity_t entity) -> void;

        auto getPlayerNameEntities() -> std::vector<entity_t>;
        auto addPlayerNameEntities(entity_t entity) -> void;
        auto removePlayerNameEntities(entity_t entity) -> void;

    private:
        Player user;
        std::vector<Player> other_players;
        network::LobbyGameState game_state = network::LobbyGameState::Waiting;
        uint32_t roomId;

        entity_t textIdEntity;
        std::vector<entity_t> textPlayerNameEntities;
    };
} // namespace game

std::optional<std::reference_wrapper<game::GameState>> getGameState(Registry &ecs);

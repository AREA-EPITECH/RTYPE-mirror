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

namespace game
{
    class GameState {
    public:
        struct Player {
            uint32_t id;
            uint16_t health = 100;
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

        auto getGameState() -> bool;
        auto updateGameState(bool state) -> void;

    private:
        Player user;
        std::vector<Player> other_players;
        bool is_playing = false;
    };
} // namespace game

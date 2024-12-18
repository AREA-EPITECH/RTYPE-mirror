/*
** EPITECH PROJECT, 2024
** RTYPE-mirror
** File description:
** GameState.cpp
*/

#include "game/GameState.hpp"

namespace game
{
    GameState::GameState() {};

    GameState::GameState(const GameState::Player &player) : user(player) {};

    GameState::~GameState() {};

    auto GameState::getUser() -> GameState::Player {
        return user;
    }

    auto GameState::updateUser(const GameState::Player &player) -> void {
        user = player;
    }

    auto GameState::getOtherPlayer() -> std::vector<GameState::Player> {
        return other_players;
    }

    auto GameState::updateOtherPlayer(const std::vector<GameState::Player> &players) -> void {
        other_players = players;
    }

    auto GameState::getGameState() -> bool {
        return is_playing;
    }

    auto GameState::updateGameState(bool state) -> void {
        is_playing = state;
    }
} // namespace game

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

    auto GameState::getGameState() -> GameState::LobbyGameState {
        return game_state;
    }

    auto GameState::updateGameState(GameState::LobbyGameState state) -> void {
        game_state = state;
    }

    auto GameState::getRoomId() -> uint32_t {
        return roomId;
    }

    auto GameState::setRoomId(uint32_t _roomId) -> void {
        roomId = _roomId;
    }

    auto GameState::getIdEntity() -> entity_t {
        return textIdEntity;
    }

    auto GameState::setIdEntity(entity_t entity) -> void {
        textIdEntity = entity;
    }

    auto GameState::getPlayerNameEntities() -> std::vector<entity_t> {
        return textPlayerNameEntities;
    }

    auto GameState::getEnemyEntities() -> std::map<uint32_t, entity_t> {
        return enemy_entities;
    }

    auto GameState::setEnemyEntities(const std::map<uint32_t, entity_t> &entities) -> void {
        enemy_entities = entities;
    }

    auto GameState::addPlayerNameEntities(entity_t entity) -> void {
        textPlayerNameEntities.push_back(entity);
    }

    auto GameState::removePlayerNameEntities(entity_t entity) -> void {
        for (auto it = textPlayerNameEntities.begin(); it != textPlayerNameEntities.end(); it++) {
            if (*it == entity) {
                textPlayerNameEntities.erase(it);
            }
        }
    }

    auto GameState::getShowScore() -> bool {
        return show_score;
    }

    auto GameState::setShowScore(bool show) -> void {
        show_score = show;
    }
} // namespace game

std::optional<std::reference_wrapper<game::GameState>> getGameState(Registry &ecs) {
    auto &gameStateCps = ecs.get_components<game::GameState>();
    std::optional<std::reference_wrapper<game::GameState>> gameState;
    for (auto &it : gameStateCps)
    {
        if (it.has_value())
        {
            gameState = std::ref(*it);
            break;
        }
    }
    return gameState;
}

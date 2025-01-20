/*
** EPITECH PROJECT, 2024
** r-type
** File description:
** DataType.hpp
*/

#pragma once

#include <map>
#include <spdlog/spdlog.h>
#include <chrono>

namespace tower_defense
{
    enum EnemyType
    {
        Basic_slime,
        Bat,
        Zombie,
    };

    inline std::map<std::string, EnemyType> string_to_enemy_type = {
        {"basic_slime", Basic_slime},
        {"bat", Bat},
        {"zombie", Zombie}
    };

    inline EnemyType get_enemy_type_from_string(const std::string &type_str)
    {
        if (string_to_enemy_type.find(type_str) != string_to_enemy_type.end()) {
            return string_to_enemy_type[type_str];
        }
        spdlog::error("Unknown enemy type string: " + type_str);
        exit(84);
    }

    enum TileType
    {
        GRASS = 0,
        PATH,
        DECOR,
        ARCHER,
        BASE_TOWER,
        BAT,
        BASIC_SLIME,
        ZOMBIE
    };

    struct EnemyWave
    {
        EnemyType _enemy_type;
        float _start_delay; // in seconds
        float _spawn_delay; // in seconds
        unsigned int _spawn_amount;
        unsigned int _amount;
        bool _finished;
        std::chrono::time_point<std::chrono::steady_clock> _time_since_last_spawn = std::chrono::steady_clock::now();
    };

    struct Position
    {
        int x;
        int y;
    };
} // namespace tower_defense

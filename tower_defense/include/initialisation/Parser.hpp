/*
** EPITECH PROJECT, 2025
** r-type
** File description:
** Map
*/

#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <filesystem>
#include <random>
#include <spdlog/spdlog.h>
#include "ecs/DataType.hpp"
#include "nlohmann/json.hpp"
#include "ecs/Systems.hpp"

namespace tower_defense {
    struct Game_rules {
        unsigned int start_money;
        unsigned int start_health;
        bool auto_increment_money;
    };

    struct EnemyWave
    {
        EnemyType enemy_type;
        float start_delay; // in seconds
        float spawn_delay; // in seconds
        unsigned int spawn_amount;
        unsigned int amount;
    };

    class Parser {
        Game_rules _game_rules;
        std::vector<std::pair<int, int>> _enemy_path;
        std::vector<std::pair<int, int>> _decorations;
        std::vector<std::vector<EnemyWave>> _enemy_waves;

        public:
            Parser() = default;
            ~Parser() = default;

            void parse_filemap(const std::string &map_filename);
            Registry &fill_ecs(Registry &ecs);
    };
}

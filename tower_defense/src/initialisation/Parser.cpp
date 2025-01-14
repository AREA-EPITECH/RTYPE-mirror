/*
** EPITECH PROJECT, 2025
** r-type
** File description:
** Parser
*/

#include "initialisation/Parser.hpp"

namespace tower_defense
{
    void Parser::parse_filemap(const std::string &map_filename)
    {
        std::ifstream ifs (map_filename);
        if (!ifs.is_open())
        {
            spdlog::error("Error: can't open following file: " + map_filename);
            exit(84);
        }
        nlohmann::json data = nlohmann::json::parse(ifs);
        nlohmann::json data_params = data["params"];
        nlohmann::json data_waves = data["waves"];

        Game_rules games_rules{};
        games_rules._auto_increment_money = data_params["auto_increment_money"];
        games_rules._start_health = data_params["health"];
        games_rules._start_money = data_params["base_money"];
        games_rules._map_name = data_params["name"];

        this->_game_rules = games_rules;

        _enemy_path.emplace_back(data_params["start_tile"][0], data_params["start_tile"][1]);
        for (auto &tile : data_params["path"])
        {
            _enemy_path.emplace_back(tile[0], tile[1]);
        }
        _enemy_path.emplace_back(data_params["end_tile"][0], data_params["end_tile"][1]);

        for (auto &decoration : data_params["decorations"])
        {
            _decorations.emplace_back(decoration[0], decoration[1]);
        }

        for (auto &wave : data_waves) {
            std::vector<EnemyWave> enemy_waves_in_wave;

            for (auto &enemy : wave["enemies"]) {
                EnemyWave enemy_wave{};
                enemy_wave._enemy_type = get_enemy_type_from_string(enemy["type"]);
                enemy_wave._start_delay = enemy["start_delay"];
                enemy_wave._spawn_delay = enemy["spawn_delay"];
                enemy_wave._spawn_amount = enemy["spawn_amount"];
                enemy_wave._amount = enemy["amount"];

                enemy_waves_in_wave.emplace_back(enemy_wave);
            }
            _enemy_waves.emplace_back(enemy_waves_in_wave);
        }
    }

    Registry &Parser::fill_ecs(Registry &ecs)
    {
        ecs::TextureManager texture_manager;

        texture_manager.add_texture(GRASS, "tower_defense/assets/maps/grass.png");
        texture_manager.add_texture(PATH, "tower_defense/assets/maps/path.png");
        texture_manager.add_texture(DECOR, "tower_defense/assets/maps/decors/rock_1.png");
        texture_manager.add_texture(DECOR, "tower_defense/assets/maps/decors/rock_2.png");
        texture_manager.add_texture(DECOR, "tower_defense/assets/maps/decors/rock_3.png");
        texture_manager.add_texture(ARCHER, "tower_defense/assets/towers/archer.png");
        texture_manager.add_texture(BASIC_SLIME, "tower_defense/assets/enemies/spr_normal_slime.png");
        texture_manager.add_texture(BAT, "tower_defense/assets/enemies/spr_bat.png");
        texture_manager.add_texture(ZOMBIE, "tower_defense/assets/enemies/spr_zombie.png");

        const auto texture_manager_entity = ecs.spawn_entity();
        ecs.add_component<ecs::TextureManager>(texture_manager_entity, std::move(texture_manager));

        std::vector<ecs::Tile> map;
        std::vector<ecs::Tile> path;
        std::vector<ecs::Tile> decors;

        ecs::Money money_component = {_game_rules._start_money, LoadTexture("tower_defense/assets/money.png")};

        ecs::GameComponent game_component = {_game_rules._start_health,
            _game_rules._auto_increment_money, _game_rules._map_name, money_component};

        for (const auto& pos : _enemy_path) {
            path.emplace_back(ecs::Tile{pos.first, pos.second, PATH, texture_manager.get_texture(PATH)});
        }

        for (const auto& pos : _decorations) {
            decors.emplace_back(ecs::Tile{pos.first, pos.second, DECOR, texture_manager.get_texture(DECOR)});
        }

        for (int y = 0; y < 8; y++)
        {
            for (int x = 0; x < 8; x++)
            {
                map.emplace_back(ecs::Tile{x, y, GRASS, texture_manager.get_texture(GRASS)});
            }
        }

        const auto map_entity = ecs.spawn_entity();
        ecs.add_component<ecs::MapComponent>(map_entity, {map, path, decors, game_component});

        const auto selector_entity = ecs.spawn_entity();
        ecs.add_component<ecs::SelectorComponent>(selector_entity,
        {LoadTexture("tower_defense/assets/selector.png"), {}, false});

        ecs::Tower tower = {2, 1, 1, 100, "Archer",
            texture_manager.get_texture(ARCHER), ARCHER};

        const auto shop_entity = ecs.spawn_entity();
        ecs.add_component<ecs::Shop>(shop_entity, {std::vector<ecs::Tower>{tower}, false});

        return ecs;
    }

}

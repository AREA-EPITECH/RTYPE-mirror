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
        std::ifstream ifs(map_filename);
        if (!ifs.is_open())
        {
            spdlog::error("Error: can't open following file: " + map_filename);
            exit(84);
        }
        nlohmann::json data = nlohmann::json::parse(ifs);
        nlohmann::json data_params = data["params"];
        nlohmann::json data_waves = data["waves"];

        Game_rules games_rules{};
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

        for (auto &wave : data_waves)
        {
            std::vector<EnemyWave> enemy_waves_in_wave;

            for (auto &enemy : wave["enemies"])
            {
                EnemyWave enemy_wave{};
                enemy_wave._enemy_type = get_enemy_type_from_string(enemy["type"]);
                enemy_wave._start_delay = enemy["start_delay"];
                enemy_wave._spawn_delay = enemy["spawn_delay"];
                enemy_wave._spawn_amount = enemy["spawn_amount"];
                enemy_wave._amount = enemy["amount"];
                enemy_wave._finished = false;

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
        texture_manager.add_texture(BASE_TOWER, "tower_defense/assets/towers/base_tower.png");
        texture_manager.add_texture(BASIC_SLIME, "tower_defense/assets/enemies/basic_slime.png");
        texture_manager.add_texture(BAT, "tower_defense/assets/enemies/bat.png");
        texture_manager.add_texture(ZOMBIE, "tower_defense/assets/enemies/zombie.png");

        const auto texture_manager_entity = ecs.spawn_entity();
        ecs.add_component<ecs::TextureManager>(texture_manager_entity, std::move(texture_manager));

        std::vector<ecs::Tile> map;
        std::vector<ecs::Tile> path;
        std::vector<ecs::Tile> decors;

        ecs::Money money_component = {_game_rules._start_money, LoadTexture("tower_defense/assets/infos/money.png")};

        ecs::Life life_component = {_game_rules._start_health, LoadTexture("tower_defense/assets/infos/life.png")};


        ecs::GameComponent game_component = {_game_rules._map_name, money_component, life_component, 3, 0.5,
                                             _enemy_waves};

        for (const auto &pos : _enemy_path)
        {
            path.emplace_back(ecs::Tile{pos.first, pos.second, PATH, texture_manager.get_texture(PATH)});
        }

        for (const auto &pos : _decorations)
        {
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
        ecs.add_component<ecs::MapComponent>(map_entity, {map, path, decors, {}, {}, game_component});

        std::vector<Rectangle> no_clickable = {};
        int scale = 4;
        Texture2D path_texture = *texture_manager.get_texture(PATH);
        Texture2D decor_texture = *texture_manager.get_texture(DECOR);


        for (auto & i : this->_enemy_path)
        {
            no_clickable.emplace_back(Rectangle{
                static_cast<float>(i.first * path_texture.width * scale),
                static_cast<float>(i.second * path_texture.height * scale),
                static_cast<float>(path_texture.width * scale), static_cast<float>(path_texture.height * scale)});
        }

        for (auto & _decoration : this->_decorations)
        {
            no_clickable.emplace_back(Rectangle{
                static_cast<float>(_decoration.first * decor_texture.width * scale),
                static_cast<float>(_decoration.second * decor_texture.height * scale),
                static_cast<float>(decor_texture.width * scale), static_cast<float>(decor_texture.height * scale)});
        }

        no_clickable.emplace_back(Rectangle{0, 0, static_cast<float>(32 * scale), static_cast<float>(32 * scale)});

        const auto selector_entity = ecs.spawn_entity();
        ecs.add_component<ecs::SelectorComponent>(
            selector_entity, {LoadTexture("tower_defense/assets/infos/selector.png"), {}, false, no_clickable});

        ecs::Tower tower = {2, 3, 1, {}, 50, "Archer", texture_manager.get_texture(ARCHER), 0, 0, ARCHER};

        const auto shop_entity = ecs.spawn_entity();
        ecs.add_component<ecs::Shop>(shop_entity, {std::vector<ecs::Tower>{tower}, false});

        return ecs;
    }

} // namespace tower_defense

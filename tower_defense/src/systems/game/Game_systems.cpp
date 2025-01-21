/*
** EPITECH PROJECT, 2025
** r-type
** File description:
** Game_systems
*/

#include "ecs/Systems.hpp"

namespace ecs
{
    /**
     * @brief Draw text components
     * @param ecs
     *
     */
    void draw_text_components(Registry &ecs)
    {
        auto &text_components = ecs.get_components<TextComponent>();

        for (int i = 0; i < text_components.size(); i++)
        {
            if (text_components[i].has_value())
            {
                std::chrono::time_point<std::chrono::steady_clock> now = std::chrono::steady_clock::now();
                double elapsed_time_since_start_round =
                    std::chrono::duration<double>(now - text_components[i]->_creation).count();
                if (elapsed_time_since_start_round >= text_components[i]->_duration)
                {
                    ecs.kill_entity(i);
                }
                else
                {
                    DrawText(text_components[i].value()._text.c_str(), text_components[i].value()._position.x,
                             text_components[i].value()._position.y, 32, text_components[i].value()._color);
                    text_components[i].value()._position.x += text_components[i].value()._velocity.x;
                    text_components[i].value()._position.y += text_components[i].value()._velocity.y;
                }
            }
        }
    }

    /**
     * @brief Check wave state
     * @param map
     */
    void check_wave(Registry &ecs, MapComponent &map)
    {
        auto &enemies = ecs.get_components<EnemyComponent>();

        if (map._game._life._health <= 0)
        {
            exit(0); // Change to game over screen -> restart game
        }

        for (auto &i : map._game._enemy_waves[map._game._current_wave])
        {
            if (i._amount != 0)
            {
                return; // Wave not finished spawned
            }
        }

        if (enemies.size() == 0 && map._game._wave_started)
        {
            map._game._current_wave++;
            map._game._wave_started = false;
        }

        if (map._game._current_wave >= map._game._enemy_waves.size())
        {
            exit(0); // Game won, change to win screen -> restart game
        }
    }

    /**
     * @brief Check the towers
     * @param ecs
     * @param map
     * @param scale
     */
    void check_towers(Registry &ecs, MapComponent &map, const int scale)
    {
        auto &towers = ecs.get_components<Tower>();
        auto &enemies = ecs.get_components<EnemyComponent>();
        auto &sounds = ecs.get_components<SoundComponent>();

        for (auto &tower : towers)
        {
            if (tower.has_value())
            {
                std::chrono::time_point<std::chrono::steady_clock> now = std::chrono::steady_clock::now();
                double elapsed_time_since_last_shot =
                    std::chrono::duration<double>(now - tower.value()._last_shot).count();

                if (elapsed_time_since_last_shot > tower.value()._fire_rate)
                {
                    EnemyComponent *target_enemy = nullptr;
                    double min_distance = tower.value()._range;
                    for (auto &enemy : enemies)
                    {
                        if (enemy.has_value())
                        {
                            const double dx = std::abs(enemy.value()._position.x - tower.value()._position.x);
                            const double dy = std::abs(enemy.value()._position.y - tower.value()._position.y);
                            const double distance = std::sqrt(dx * dx + dy * dy);

                            if (distance <= tower.value()._range && distance < min_distance)
                            {
                                target_enemy = &enemy.value();
                                min_distance = distance;
                            }
                            for (auto &sound : sounds)
                            {
                                if (sound.has_value())
                                {
                                    switch (tower.value()._tower_type)
                                    {
                                    case tower_defense::ARCHER:
                                        sound.value().play(tower_defense::ARCHER_SHOT);
                                        break;
                                    default:
                                        break;
                                    }
                                }
                            }
                        }
                    }

                    if (target_enemy)
                    {
                        target_enemy->_health -= tower.value()._damage;
                        ecs.run_event(CreateTextEvent{"-" + std::to_string(tower.value()._damage), 1,
                                                      target_enemy->_position.x * 32 * scale,
                                                      target_enemy->_position.y * 32 * scale, 0, -1, RED, 32});
                        tower.value()._last_shot = std::chrono::steady_clock::now();
                    }
                }
            }
        }
    }


    /**
     * @brief Draw the towers
     * @param ecs
     * @param map
     * @param scale
     * @param tm
     */
    void draw_towers(Registry &ecs, MapComponent &map, const float scale, TextureManager &tm)
    {
        auto &towers = ecs.get_components<Tower>();
        for (auto &tower : towers)
        {
            if (tower.has_value())
            {
                const Texture2D tower_texture = *tower.value()._texture;
                const Texture2D base_texture = *tm.get_texture(tower_defense::BASE_TOWER);

                DrawTextureEx(base_texture,
                              {static_cast<float>(tower.value()._position.x * base_texture.width) * scale,
                               static_cast<float>(tower.value()._position.y * base_texture.height) * scale},
                              0, scale, WHITE);

                const Rectangle source_rect = {0.0f, static_cast<float>(2 * 32), 32.0f, 32.0f};
                const Rectangle dest_rect = {static_cast<float>(tower.value()._position.x * 32) * scale,
                                             static_cast<float>(tower.value()._position.y * 32) * scale,
                                             source_rect.width * scale, source_rect.height * scale};
                DrawTexturePro(tower_texture, source_rect, dest_rect, {0, 0}, 0.0f, WHITE);
            }
        }
    }


    /**
     * @brief
     * @param ecs
     * @param map
     * @param scale
     */
    void check_enemies(Registry &ecs, MapComponent &map, const int scale)
    {
        auto &enemies = ecs.get_components<EnemyComponent>();
        auto &sounds = ecs.get_components<SoundComponent>();

        for (int i = 0; i < enemies.size(); i++)
        {
            if (enemies[i].has_value())
            {
                if (enemies[i].value()._health <= 0)
                {
                    map._game._money._value += enemies[i].value()._reward;
                    ecs.run_event(CreateTextEvent{"+" + std::to_string(enemies[i].value()._reward), 1,
                                                  enemies[i].value()._position.x * 32 * scale,
                                                  enemies[i].value()._position.y * 32 * scale, 0, -1, GOLD, 32});
                    for (auto &sound : sounds)
                    {
                        if (sound.has_value())
                        {
                            switch (enemies[i].value()._enemy_type)
                            {
                            case tower_defense::Bat:
                                sound.value().play(tower_defense::BAT_DEATH);
                                break;
                            case tower_defense::Basic_slime:
                                sound.value().play(tower_defense::BASIC_SLIME_DEATH);
                                break;
                            case tower_defense::Zombie:
                                sound.value().play(tower_defense::ZOMBIE_DEATH);
                                break;
                            default:
                                break;
                            }
                        }
                    }
                    ecs.kill_entity(i);
                    continue;
                }
                if (enemies[i].value()._position.x == map._path[map._path.size() - 1]._position.x &&
                    enemies[i].value()._position.y == map._path[map._path.size() - 1]._position.y)
                {

                    map._game._life._health -= enemies[i].value()._damage;
                    ecs.run_event(CreateTextEvent{"-" + std::to_string(enemies[i].value()._damage),
                                                  1,
                                                  enemies[i].value()._position.x * 32 * scale,
                                                  enemies[i].value()._position.y * 32 * scale,
                                                  0,
                                                  -1,
                                                  {136, 8, 8, 255},
                                                  32});
                    ecs.kill_entity(i);
                    for (auto &sound : sounds)
                    {
                        if (sound.has_value())
                        {
                            sound.value().play(tower_defense::LIFE_LOST);
                        }
                    }
                }
            }
        }
    }

    /**
     * @brief Update the enemies position
     * @param ecs
     * @param map
     */
    void update_enemies_pos(Registry &ecs, MapComponent &map)
    {
        auto &enemies = ecs.get_components<EnemyComponent>();

        for (auto &enemy : enemies)
        {
            if (enemy.has_value())
            {
                std::chrono::time_point<std::chrono::steady_clock> now = std::chrono::steady_clock::now();
                double elapsed_time_since_start_round =
                    std::chrono::duration<double>(now - enemy.value()._last_move).count();
                if (elapsed_time_since_start_round < enemy.value()._speed)
                {
                    continue;
                }
                {
                    for (size_t j = 0; j < map._path.size(); j++)
                    {
                        if (map._path[j]._position.x == enemy.value()._position.x &&
                            map._path[j]._position.y == enemy.value()._position.y)
                        {
                            if (j + 1 < map._path.size())
                            {
                                enemy.value()._position.x = map._path[j + 1]._position.x;
                                enemy.value()._position.y = map._path[j + 1]._position.y;
                            }
                            enemy.value()._last_move = std::chrono::steady_clock::now();
                            break;
                        }
                    }
                }
            }
        }
    }

    /**
     * @brief Update the enemies position
     * @param ecs
     * @param map
     * @param texture_manager
     */
    void spawn_enemies(Registry &ecs, MapComponent &map, TextureManager &texture_manager)
    {
        if (!map._game._wave_started)
        {
            return;
        }
        for (int i = 0; i < map._game._enemy_waves[map._game._current_wave].size(); i++)
        {
            if (map._game._enemy_waves[map._game._current_wave][i]._amount > 0)
            {
                std::chrono::time_point<std::chrono::steady_clock> now = std::chrono::steady_clock::now();
                const double elapsed_time_since_start_round =
                    std::chrono::duration<double>(now - map._game._wave_start_time).count();
                const double elapsed_time_since_last_spawn =
                    std::chrono::duration<double>(
                        now - map._game._enemy_waves[map._game._current_wave][i]._time_since_last_spawn)
                        .count();

                if (elapsed_time_since_start_round < map._game._enemy_waves[map._game._current_wave][i]._start_delay ||
                    elapsed_time_since_last_spawn < map._game._enemy_waves[map._game._current_wave][i]._spawn_delay)
                {
                    continue;
                }

                for (int amount = 0; amount < map._game._enemy_waves[map._game._current_wave][i]._spawn_amount;
                     amount++)
                {
                    switch (map._game._enemy_waves[map._game._current_wave][i]._enemy_type)
                    {
                    case tower_defense::Basic_slime:
                        ecs.run_event(CreateEnemyEvent{10, 2, 1, 10,
                                                       texture_manager.get_texture(tower_defense::BASIC_SLIME),
                                                       map._path[0]._position, tower_defense::Basic_slime});
                        break;
                    case tower_defense::Bat:
                        ecs.run_event(CreateEnemyEvent{5, 1, 2, 5, texture_manager.get_texture(tower_defense::BAT),
                                                       map._path[0]._position, tower_defense::Bat});
                        break;
                    case tower_defense::Zombie:
                        ecs.run_event(CreateEnemyEvent{15, 3, 5, 15, texture_manager.get_texture(tower_defense::ZOMBIE),
                                                       map._path[0]._position, tower_defense::Zombie});
                        break;
                    default:
                        break;
                    }
                }
                map._game._enemy_waves[map._game._current_wave][i]._amount -=
                    map._game._enemy_waves[map._game._current_wave][i]._spawn_amount;
                map._game._enemy_waves[map._game._current_wave][i]._time_since_last_spawn =
                    std::chrono::steady_clock::now();
                return;
            }
        }
    }

    /**
     * @brief Draw the enemies
     * @param ecs
     * @param map
     * @param scale
     */
    void draw_enemies(Registry &ecs, MapComponent &map, const float scale)
    {
        auto &enemies = ecs.get_components<EnemyComponent>();

        for (auto &enemy : enemies)
        {
            if (enemy.has_value())
            {
                const Texture2D texture = *enemy.value()._texture;

                enemy.value()._frame_counter += map._game._frame_time;
                if (enemy.value()._frame_counter >= map._game._frame_time * 15)
                {
                    enemy.value()._frame_counter = 0.0f;
                    enemy.value()._frame++;

                    if (enemy.value()._frame >= 4)
                    {
                        enemy.value()._frame = 0;
                    }
                }
                const Rectangle source_rect = {0.0f, static_cast<float>(enemy.value()._frame * 32), 32, 32};
                const Rectangle dest_rect = {static_cast<float>(enemy.value()._position.x * 32) * scale,
                                             static_cast<float>(enemy.value()._position.y * 32) * scale,
                                             source_rect.width * static_cast<float>(scale),
                                             source_rect.height * static_cast<float>(scale)};

                DrawTexturePro(texture, source_rect, dest_rect, {0.0f, 0.0f}, 0.0f, WHITE);
            }
        }
    }

    /**
     * @brief Draw the game infos
     * @param ecs
     * @param selector
     * @param map
     * @param scale
     */
    void draw_game_infos(Registry &ecs, const SelectorComponent &selector, MapComponent &map, const float scale)
    {
        auto &towers = ecs.get_components<Tower>();

        for (const auto &tower : towers)
        {
            if (tower.has_value())
            {
                Tower t = tower.value();
                if (selector._position.x == t._position.x && selector._position.y == t._position.y)
                {
                    DrawCircle(static_cast<int>(static_cast<float>(selector._position.x) * 32.0f * scale +
                                                (32.0f * scale / 2.0f)),
                               static_cast<int>(static_cast<float>(selector._position.y) * 32.0f * scale +
                                                (32.0f * scale / 2.0f)),
                               (static_cast<float>(t._range) * 32.0f * scale) + (32.0f * scale / 2.0f),
                               Fade(GRAY, 0.3f));
                }
            }
        }

        if (selector._drawable)
        {
            DrawTextureEx(selector._texture,
                          {static_cast<float>(selector._texture.width * selector._position.x) * scale,
                           static_cast<float>(selector._texture.height * selector._position.y) * scale},
                          0, 4, WHITE);
        }

        DrawText(map._game._map_name.c_str(), 10, GetScreenHeight() - 30, 32, RED);
        DrawText(TextFormat("Round: %d / %d", map._game._current_wave + 1, map._game._enemy_waves.size()), 10,
                 GetScreenHeight() - 60, 32, RED);

        DrawTextureEx(map._game._money._texture,
                      {2, static_cast<float>(GetScreenHeight() - 130 - map._game._money._texture.height)}, 0, 4, WHITE);
        DrawText(std::to_string(map._game._money._value).c_str(), 5 + map._game._money._texture.width * 2,
                 GetScreenHeight() - 80 - map._game._money._texture.height, 32, GOLD);

        DrawTextureEx(map._game._life._texture,
                      {2, static_cast<float>(GetScreenHeight() - 200 - map._game._life._texture.height)}, 0, 4, WHITE);

        const std::string str_life = std::to_string(map._game._life._health);
        DrawText(str_life.c_str(),
                 static_cast<int>(map._game._life._texture.width * 1.5 - static_cast<double>(str_life.length())),
                 GetScreenHeight() - 150 - map._game._life._texture.height, 32, WHITE);

        if (!map._game._wave_started)
        {
            GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, ColorToInt(GREEN));
        }
        else
        {
            GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, ColorToInt(GRAY));
        }

        std::string str_button_round = "Start round";
        if (!map._game._wave_started)
        {
            if (GuiButton({static_cast<float>(GetScreenWidth() - str_button_round.length() * 10),
                           static_cast<float>(GetScreenHeight() - 50), 100, 40},
                          str_button_round.c_str()))
            {
                map._game._wave_started = true;
                map._game._wave_start_time = std::chrono::steady_clock::now();
            }
        }
        GuiButton({static_cast<float>(GetScreenWidth() - str_button_round.length() * 10),
                   static_cast<float>(GetScreenHeight() - 50), 100, 40},
                  str_button_round.c_str());
        GuiLoadStyleDefault(); // Reset style
    }

    /**
     * @brief Draw the map
     * @param map
     * @param scale
     */
    void draw_map(MapComponent &map, const float scale)
    {

        for (auto &g : map._grass)
        {
            const Texture2D texture = *g._texture;
            DrawTextureEx(
                texture,
                {32 * scale * static_cast<float>(g._position.x), 32 * scale * static_cast<float>(g._position.y)}, 0, 4,
                WHITE);
        }
        for (auto &p : map._path)
        {
            const Texture2D texture = *p._texture;
            DrawTextureEx(
                texture,
                {32 * scale * static_cast<float>(p._position.x), 32 * scale * static_cast<float>(p._position.y)}, 0,
                scale, WHITE);
        }
        for (auto &d : map._decors)
        {
            const Texture2D texture = *d._texture;
            DrawTextureEx(
                texture,
                {32 * scale * static_cast<float>(d._position.x), 32 * scale * static_cast<float>(d._position.y)}, 0,
                scale, WHITE);
        }
    }

    /**
     * @brief Handle the shop
     * @param ecs
     * @param scale
     * @param map
     */
    void handle_shop(Registry &ecs, const float scale, MapComponent &map)
    {
        auto &shops = ecs.get_components<Shop>();
        auto &selectors = ecs.get_components<SelectorComponent>();
        auto &towers = ecs.get_components<Tower>();
        auto &sounds = ecs.get_components<SoundComponent>();

        for (auto &shop : shops)
        {
            if (shop.has_value())
            {
                auto &s = shop.value();
                Rectangle box = {24, 24, 100, 30};
                if (GuiButton(box, "Shop"))
                {
                    s._open = !s._open;
                }
                if (s._open)
                {
                    for (auto &selector : selectors)
                    {
                        if (selector.has_value())
                        {
                            box = {10, 100, 200, 200};
                            GuiLabel(box, "Shop");
                            for (int i = 0; i < s._towers.size(); i++)
                            {
                                Tower &tower = s._towers[i];

                                const float x = static_cast<float>(20) + static_cast<float>(i % 3) * 260;
                                const float y = static_cast<float>(60) + static_cast<float>(i) / 3 * 160;

                                GuiGroupBox((Rectangle){static_cast<float>(x), static_cast<float>(y), 240, 140}, "");

                                box = {x + 10, y + 10, 200, 20};
                                GuiLabel(box, tower._name.c_str());
                                box = {x + 10, y + 40, 200, 20};
                                GuiLabel(box}, TextFormat("Price : %d", tower._cost));
                                box = {x + 10, y + 70, 200, 20};
                                GuiLabel(box, TextFormat("Range : %d", tower._range));
                                box = {x + 10, y + 100, 100, 30};
                                if (GuiButton(box, "Buy"))
                                {
                                    bool purchasable = true;
                                    if (!selector.value()._drawable)
                                    {
                                        purchasable = false; // No tile selected
                                    }
                                    for (const auto &tower_component : towers)
                                    {
                                        if (tower_component.has_value())
                                        {
                                            const Tower &t = tower_component.value();
                                            if (selector.value()._position.x == t._position.x &&
                                                selector.value()._position.y == t._position.y)
                                            {
                                                purchasable = false; // Tower already here
                                            }
                                        }
                                    }
                                    if (map._game._money._value >= tower._cost && purchasable)
                                    {
                                        map._game._money._value -= tower._cost;
                                        ecs.run_event(CreateTowerEvent{tower._range, tower._damage, tower._fire_rate,
                                                                       tower._cost, tower._name, tower._texture,
                                                                       tower._tower_type, selector.value()._position});
                                        selector.value()._drawable = false;
                                        for (auto &sound: sounds)
                                        {
                                            if (sound.has_value())
                                            {
                                                sound.value().play(tower_defense::TOWER_BUILT);
                                            }
                                        }
                                    }
                                }
                                const Texture2D texture = *tower._texture;

                                const Rectangle source_rect = {0.0f, static_cast<float>(2 * 32), 32.0f, 32.0f};
                                const Rectangle dest_rect = {x + 100, y + 10,
                                                             source_rect.width * static_cast<float>(scale),
                                                             source_rect.height * static_cast<float>(scale)};
                                DrawTexturePro(texture, source_rect, dest_rect, {0, 0}, 0.0f, WHITE);
                            }
                        }
                    }
                }
            }
        }
    }

    /**
     * @brief Draw the game
     * @param ecs
     */
    void draw_game_system(Registry &ecs, const WindowDrawEvent &)
    {
        ecs.run_event(ControlsEvent{});
        auto &windows = ecs.get_components<Window>();
        auto &maps = ecs.get_components<MapComponent>();
        auto &selectors = ecs.get_components<SelectorComponent>();
        auto &textures_managers = ecs.get_components<TextureManager>();
        auto &musics = ecs.get_components<MusicComponent>();

        SelectorComponent s = {};
        TextureManager tm = {};

        for (auto &selector : selectors)
        {
            if (selector.has_value())
            {
                s = selector.value();
                break;
            }
        }

        for (auto &texture_manager : textures_managers)
        {
            if (texture_manager.has_value())
            {
                tm = texture_manager.value();
                break;
            }
        }

        for (auto &window : windows)
        {
            if (window.has_value())
            {
                auto &win = window.value();
                if (win.isOpen)
                {
                    for (auto &music: musics)
                    {
                        if (music.has_value())
                        {
                            if (!music.value().isPlaying(tower_defense::GAME_MUSIC))
                                music.value().play(tower_defense::GAME_MUSIC);
                            music.value().update(tower_defense::GAME_MUSIC);
                        }
                    }

                    BeginDrawing();
                    ClearBackground(RAYWHITE);

                    for (auto &map : maps)
                    {
                        if (map.has_value())
                        {
                            auto &m = map.value();
                            const float scale = 4;

                            check_wave(ecs, m);
                            draw_map(m, scale);
                            draw_game_infos(ecs, s, m, scale);
                            draw_enemies(ecs, m, scale);
                            draw_towers(ecs, m, scale, tm);
                            draw_text_components(ecs);
                            handle_shop(ecs, scale, m);
                            check_enemies(ecs, m, static_cast<int>(scale));
                            update_enemies_pos(ecs, m);
                            spawn_enemies(ecs, m, tm);
                            check_towers(ecs, m, static_cast<int>(scale));
                        }
                    }
                    EndDrawing();
                }
            }
        }
    }
} // namespace ecs

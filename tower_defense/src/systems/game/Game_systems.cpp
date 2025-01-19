/*
** EPITECH PROJECT, 2025
** r-type
** File description:
** Game_systems
*/

#include "ecs/Systems.hpp"

namespace ecs
{
    void check_wave(MapComponent &map)
    {
        if (map._game._life._health <= 0)
        {
            exit(0); // Change to game over screen -> restart game
        }

        for (int i = 0; i < map._game._enemy_waves[map._game._current_wave].size(); i++)
        {
            if (map._game._enemy_waves[map._game._current_wave][i]._amount != 0)
            {
                return; // Wave not finished spawned
            }
        }

        if (map._enemies.empty())
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
     * @param map
     */
    void check_towers(MapComponent &map)
    {
        for (Tower &tower : map._towers)
        {
            std::chrono::time_point<std::chrono::steady_clock> now = std::chrono::steady_clock::now();
            double elapsed_time_since_last_shot = std::chrono::duration<double>(now - tower._last_shot).count();

            if (elapsed_time_since_last_shot > tower._fire_rate)
            {
                EnemyComponent* target_enemy = nullptr;
                double min_distance = tower._range;

                for (EnemyComponent &enemy : map._enemies)
                {

                    const double dx = std::abs(enemy._pos_x - tower._pos._x);
                    const double dy = std::abs(enemy._pos_y - tower._pos._y);
                    const double distance = std::sqrt(dx * dx + dy * dy);

                    if (distance <= tower._range && distance < min_distance)
                    {
                        target_enemy = &enemy;
                        min_distance = distance;
                    }
                }

                if (target_enemy)
                {
                    target_enemy->_health -= tower._damage;
                    tower._last_shot = std::chrono::steady_clock::now();
                }
            }
        }
    }


    /**
     * @brief Draw the towers
     * @param map
     * @param scale
     * @param tm
     */
    void draw_towers(MapComponent &map, const int scale, TextureManager &tm)
    {
        for (auto &tower : map._towers)
        {
            const Texture2D tower_texture = *tower._texture;
            const Texture2D base_texture = *tm.get_texture(tower_defense::BASE_TOWER);

            DrawTextureEx(base_texture,
                          {static_cast<float>(tower._pos._x * base_texture.width * scale),
                           static_cast<float>(tower._pos._y * base_texture.height * scale)},
                          0, scale, WHITE);

            Rectangle sourceRect = {0.0f, static_cast<float>(2 * 32), 32.0f, 32.0f};
            Rectangle destRect = {static_cast<float>(tower._pos._x * 32 * scale),
                                  static_cast<float>(tower._pos._y * 32 * scale), sourceRect.width * scale,
                                  sourceRect.height * scale};
            DrawTexturePro(tower_texture, sourceRect, destRect, {0, 0}, 0.0f, WHITE);
        }
    }


    /**
     * @brief
     * @param map
     */
    void check_enemies(MapComponent &map)
    {
        for (int i = 0; i < map._enemies.size(); i++)
        {
            if (map._enemies[i]._health <= 0)
            {
                map._game._money._value += map._enemies[i]._reward;
                map._enemies.erase(map._enemies.begin() + i);
                continue;
            }
            if (map._enemies[i]._pos_x == map._path[map._path.size() - 1]._x &&
                map._enemies[i]._pos_y == map._path[map._path.size() - 1]._y)
            {
                map._game._life._health -= map._enemies[i]._damage;
                map._enemies.erase(map._enemies.begin() + i);
            }
        }
    }

    /**
     * @brief Update the enemies position
     * @param map
     */
    void update_enemies_pos(MapComponent &map)
    {
        for (auto &enemy : map._enemies)
        {
            std::chrono::time_point<std::chrono::steady_clock> now = std::chrono::steady_clock::now();
            double elapsed_time_since_start_round = std::chrono::duration<double>(now - enemy._last_move).count();
            if (elapsed_time_since_start_round < enemy._speed)
            {
                continue;
            }
            {
                for (size_t j = 0; j < map._path.size(); j++)
                {
                    if (map._path[j]._x == enemy._pos_x && map._path[j]._y == enemy._pos_y)
                    {
                        if (j + 1 < map._path.size())
                        {
                            enemy._pos_x = map._path[j + 1]._x;
                            enemy._pos_y = map._path[j + 1]._y;
                        }
                        enemy._last_move = std::chrono::steady_clock::now();
                        break;
                    }
                }
            }
        }
    }

    /**
     * @brief Update the enemies position
     * @param map
     * @param texture_manager
     */
    void spawn_enemies(MapComponent &map, TextureManager &texture_manager)
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
                double elapsed_time_since_start_round =
                    std::chrono::duration<double>(now - map._game._wave_start_time).count();
                double elapsed_time_since_last_spawn =
                    std::chrono::duration<double>(
                        now - map._game._enemy_waves[map._game._current_wave][i]._time_since_last_spawn)
                        .count();

                if (elapsed_time_since_start_round < map._game._enemy_waves[map._game._current_wave][i]._start_delay ||
                    elapsed_time_since_last_spawn < map._game._enemy_waves[map._game._current_wave][i]._spawn_delay)
                {
                    continue;
                }

                int start_x = map._path[0]._x;
                int start_y = map._path[0]._y;

                for (int amount = 0; amount < map._game._enemy_waves[map._game._current_wave][i]._spawn_amount;
                     amount++)
                {
                    switch (map._game._enemy_waves[map._game._current_wave][i]._enemy_type)
                    {
                    case tower_defense::Basic_slime:
                        map._enemies.emplace_back(
                            EnemyComponent{10, 2, 1, 10, texture_manager.get_texture(tower_defense::BASIC_SLIME), start_x,
                                           start_y, 0, 0});
                        break;
                    case tower_defense::Bat:
                        map._enemies.emplace_back(EnemyComponent{
                            5, 3, 2, 5, texture_manager.get_texture(tower_defense::BAT), start_x, start_y, 0, 0});
                        break;
                    case tower_defense::Zombie:
                        map._enemies.emplace_back(EnemyComponent{
                            15, 1, 3, 15, texture_manager.get_texture(tower_defense::ZOMBIE), start_x, start_y, 0, 0});
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
     * @param map
     * @param scale
     */
    void draw_enemies(MapComponent &map, const int scale)
    {
        for (auto &enemy : map._enemies)
        {
            const Texture2D texture = *enemy._texture;

            enemy._frame_counter += map._game._frame_time;
            if (enemy._frame_counter >= map._game._frame_time * 15)
            {
                enemy._frame_counter = 0.0f;
                enemy._frame++;

                if (enemy._frame >= 4)
                {
                    enemy._frame = 0;
                }
            }
            Rectangle sourceRect = {0.0f, static_cast<float>(enemy._frame * 32), 32, 32};
            Rectangle destRect = {
                static_cast<float>(enemy._pos_x * 32 * scale), static_cast<float>(enemy._pos_y * 32 * scale),
                sourceRect.width * static_cast<float>(scale), sourceRect.height * static_cast<float>(scale)};

            Vector2 origin = {0.0f, 0.0f};
            DrawTexturePro(texture, sourceRect, destRect, origin, 0.0f, WHITE);
        }
    }

    /**
     * @brief Draw the game infos
     * @param selector
     * @param map
     * @param scale
     */
    void draw_game_infos(SelectorComponent &selector, MapComponent &map, const int scale)
    {
        for (int i = 0; i < map._towers.size(); i++)
        {
            if (selector._pos._x == map._towers[i]._pos._x && selector._pos._y == map._towers[i]._pos._y)
            {
                DrawCircle(selector._pos._x * 32 * scale + (32 * scale / 2),
                           selector._pos._y * 32 * scale + (32 * scale / 2),
                           map._towers[i]._range * 32 * scale + (32 * scale / 2), Fade(GRAY, 0.3f));
            }
        }

        if (selector._drawable)
        {
            DrawTextureEx(selector._texture,
                          {static_cast<float>(selector._texture.width * scale * selector._pos._x),
                           static_cast<float>(selector._texture.height * scale * selector._pos._y)},
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
    void draw_map(MapComponent &map, const int scale)
    {

        for (auto &g : map._grass)
        {
            const Texture2D texture = *g._texture;
            DrawTextureEx(texture, {static_cast<float>(32 * scale * g._x), static_cast<float>(32 * scale * g._y)}, 0, 4,
                          WHITE);
        }
        for (auto &p : map._path)
        {
            const Texture2D texture = *p._texture;
            DrawTextureEx(texture, {static_cast<float>(32 * scale * p._x), static_cast<float>(32 * scale * p._y)}, 0,
                          scale, WHITE);
        }
        for (auto &d : map._decors)
        {
            const Texture2D texture = *d._texture;
            DrawTextureEx(texture, {static_cast<float>(32 * scale * d._x), static_cast<float>(32 * scale * d._y)}, 0,
                          scale, WHITE);
        }
    }

    /**
     * @brief Handle the shop
     * @param ecs
     * @param scale
     * @param game_frame_time
     */
    void handle_shop(Registry &ecs, const int scale, const float game_frame_time, MapComponent &map)
    {
        auto &shops = ecs.get_components<Shop>();
        auto &selectors = ecs.get_components<SelectorComponent>();

        for (auto &shop : shops)
        {
            if (shop.has_value())
            {
                auto &s = shop.value();
                if (GuiButton((Rectangle){24, 24, 100, 30}, "Shop"))
                {
                    s._open = !s._open;
                }
                if (s._open)
                {
                    for (auto &selector : selectors)
                    {
                        if (selector.has_value())
                        {
                            GuiLabel((Rectangle){10, 100, 200, 200}, "Shop");
                            for (int i = 0; i < s._towers.size(); i++)
                            {
                                Tower &tower = s._towers[i];

                                const float x = static_cast<float>(20) + static_cast<float>(i % 3) * 260;
                                const float y = static_cast<float>(60) + static_cast<float>(i / 3) * 160;

                                GuiGroupBox((Rectangle){static_cast<float>(x), static_cast<float>(y), 240, 140}, "");

                                GuiLabel((Rectangle){x + 10, y + 10, 200, 20}, tower._name.c_str());
                                GuiLabel((Rectangle){x + 10, y + 40, 200, 20}, TextFormat("Price : %d", tower._cost));
                                GuiLabel((Rectangle){x + 10, y + 70, 200, 20}, TextFormat("Range : %d", tower._range));

                                if (GuiButton((Rectangle){x + 10, y + 100, 100, 30}, "Buy"))
                                {
                                    bool purchasable = true;
                                    if (!selector.value()._drawable)
                                    {
                                        purchasable = false; // No tile selected
                                    }
                                    for (int i = 0; i < map._towers.size(); i++)
                                    {
                                        if (selector.value()._pos._x == map._towers[i]._pos._x &&
                                            selector.value()._pos._y == map._towers[i]._pos._y)
                                        {
                                            purchasable = false; // Tower already here
                                        }
                                    }
                                    if (map._game._money._value >= tower._cost && purchasable)
                                    {
                                        map._game._money._value -= tower._cost;
                                        map._towers.emplace_back(
                                            Tower{tower._range,
                                                  tower._damage,
                                                  tower._fire_rate,
                                                  std::chrono::steady_clock::now(),
                                                  tower._cost,
                                                  tower._name,
                                                  tower._texture,
                                                  0,
                                                  0.0f,
                                                  tower._tower_type,
                                                  {selector.value()._pos._x, selector.value()._pos._y}});
                                        selector.value()._drawable = false;
                                    }
                                }
                                const Texture2D texture = *tower._texture;

                                Rectangle source_rect = {0.0f, static_cast<float>(2 * 32), 32.0f, 32.0f};
                                Rectangle dest_rect = {x + 100, y + 10, source_rect.width * static_cast<float>(scale),
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
                    BeginDrawing();
                    ClearBackground(RAYWHITE);

                    for (auto &map : maps)
                    {
                        if (map.has_value())
                        {
                            auto &m = map.value();
                            const int scale = 4;

                            check_wave(m);
                            draw_map(m, scale);
                            draw_game_infos(s, m, scale);
                            draw_enemies(m, scale);
                            draw_towers(m, scale, tm);
                            handle_shop(ecs, scale, m._game._frame_time, m);
                            check_enemies(m);
                            update_enemies_pos(m);
                            spawn_enemies(m, tm);
                            check_towers(m);
                        }
                    }
                    EndDrawing();
                }
            }
        }
    }
} // namespace ecs

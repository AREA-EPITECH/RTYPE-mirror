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
     * @brief
     * @param map
     */
    void check_enemies_path(MapComponent &map)
    {
        for (int i = 0; i < map._enemies.size(); i++)
        {
            if (map._enemies[i]._pos_x == map._path[map._path.size() - 1]._x &&
                map._enemies[i]._pos_y == map._path[map._path.size() - 1]._y)
            {
                map._game._life._health -= map._enemies[i]._damage;
                map._enemies.erase(map._enemies.begin() + i);
                break;
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
                            EnemyComponent{1, 3, 1, 1, texture_manager.get_texture(tower_defense::BASIC_SLIME), start_x,
                                           start_y, 0, 0});
                        break;
                    case tower_defense::Bat:
                        map._enemies.emplace_back(EnemyComponent{
                            1, 2, 2, 1, texture_manager.get_texture(tower_defense::BAT), start_x, start_y, 0, 0});
                        break;
                    case tower_defense::Zombie:
                        map._enemies.emplace_back(EnemyComponent{
                            1, 3, 3, 1, texture_manager.get_texture(tower_defense::ZOMBIE), start_x, start_y, 0, 0});
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
    void draw_enemies(MapComponent &map, int scale)
    {
        for (auto &enemy : map._enemies)
        {
            const Texture2D texture = *enemy._texture;

            enemy._frame_counter += map._game._frame_time;
            if (enemy._frame_counter >= map._game._frame_time * 15)
            {
                enemy._frame_counter = 0.0f;
                enemy._frame++;

                if (enemy._frame >= 3)
                {
                    enemy._frame = 0;
                }
            }

            Rectangle sourceRect = {0.0f, static_cast<float>(enemy._frame * enemy._texture->height),
                                    static_cast<float>(enemy._texture->width),
                                    static_cast<float>(enemy._texture->height)};
            Rectangle destRect = {static_cast<float>(enemy._pos_x * enemy._texture->width * scale),
                                  static_cast<float>(enemy._pos_y * enemy._texture->height * scale),
                                  sourceRect.width * static_cast<float>(scale),
                                  sourceRect.height * static_cast<float>(scale)};

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
        if (selector._drawable)
        {
            DrawTextureEx(selector._texture,
                          {static_cast<float>(selector._texture.width * scale * selector._pos._x),
                           static_cast<float>(selector._texture.height * scale * selector._pos._y)},
                          0, 4, WHITE);
        }

        DrawText(map._game._map_name.c_str(), 10, GetScreenHeight() - 30, 32, RED);
        DrawText(TextFormat("Round: %d / %d", map._game._current_wave, map._game._enemy_waves.size()), 10,
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
    void handle_shop(Registry &ecs, const int scale, const float game_frame_time)
    {
        auto &shops = ecs.get_components<Shop>();

        for (auto &shop : shops)
        {
            if (shop.has_value())
            {
                auto &s = shop.value();
                if (GuiButton((Rectangle){24, 24, 120, 30}, "Shop"))
                {
                    s._open = !s._open;
                }
                if (s._open)
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
                            // Do something
                        }
                        const Texture2D texture = *tower._texture;

                        tower._frame_counter += game_frame_time;
                        if (tower._frame_counter >= game_frame_time * 15)
                        {
                            tower._frame_counter = 0.0f;
                            tower._frame++;

                            if (tower._frame >= 3)
                            {
                                tower._frame = 0;
                            }
                        }

                        Rectangle sourceRect = {0.0f, static_cast<float>(tower._frame * 32), 32.0f, 32.0f};
                        Rectangle destRect = {x + 100, y + 10, sourceRect.width * scale, sourceRect.height * scale};
                        DrawTexturePro(texture, sourceRect, destRect, {0, 0}, 0.0f, WHITE);
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

                            if (m._game._life._health <= 0)
                            {
                                exit(84);
                            }
                            draw_map(m, scale);
                            draw_game_infos(s, m, scale);
                            handle_shop(ecs, scale, m._game._frame_time);
                            draw_enemies(m, scale);
                            check_enemies_path(m);
                            update_enemies_pos(m);

                            // Check if wave is clear

                            spawn_enemies(m, tm);

                            // Game logic
                        }
                    }
                    EndDrawing();
                }
            }
        }
    }
} // namespace ecs

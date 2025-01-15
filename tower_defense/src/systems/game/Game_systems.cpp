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
     * @brief Draw the game
     * @param ecs
     */
    void draw_game_system(Registry &ecs, const WindowDrawEvent &)
    {
        ecs.run_event(ControlsEvent{});
        auto &windows = ecs.get_components<Window>();
        auto &maps = ecs.get_components<MapComponent>();
        auto &selectors = ecs.get_components<SelectorComponent>();
        auto &shops = ecs.get_components<Shop>();
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

                            for (auto &g : m._grass)
                            {
                                const Texture2D texture = *g._texture;
                                DrawTextureEx(texture,
                                              {static_cast<float>(texture.width * scale * g._x),
                                               static_cast<float>(texture.height * scale * g._y)},
                                              0, 4, WHITE);
                            }
                            for (auto &p : m._path)
                            {
                                const Texture2D texture = *p._texture;
                                DrawTextureEx(texture,
                                              {static_cast<float>(texture.width * scale * p._x),
                                               static_cast<float>(texture.height * scale * p._y)},
                                              0, 4, WHITE);
                            }
                            for (auto &d : m._decors)
                            {
                                const Texture2D texture = *d._texture;
                                DrawTextureEx(texture,
                                              {static_cast<float>(texture.width * scale * d._x),
                                               static_cast<float>(texture.height * scale * d._y)},
                                              0, 4, WHITE);
                            }

                            if (s._drawable)
                            {
                                DrawTextureEx(s._texture,
                                              {static_cast<float>(s._texture.width * scale * s._pos._x),
                                               static_cast<float>(s._texture.height * scale * s._pos._y)},
                                              0, 4, WHITE);
                            }

                            DrawText(m._game._map_name.c_str(), 10, GetScreenHeight() - 30, 32, RED);
                            DrawText(TextFormat("Round: %d / %d", m._game._current_wave, m._game._enemy_waves.size()),
                                     10, GetScreenHeight() - 60, 32, RED);

                            DrawTextureEx(
                                m._game._money._texture,
                                {2, static_cast<float>(GetScreenHeight() - 130 - m._game._money._texture.height)}, 0, 4,
                                WHITE);
                            DrawText(std::to_string(m._game._money._value).c_str(),
                                     5 + m._game._money._texture.width * 2,
                                     GetScreenHeight() - 80 - m._game._money._texture.height, 32, GOLD);

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

                                            GuiGroupBox(
                                                (Rectangle){static_cast<float>(x), static_cast<float>(y), 240, 140},
                                                "");

                                            GuiLabel((Rectangle){x + 10, y + 10, 200, 20}, tower._name.c_str());
                                            GuiLabel((Rectangle){x + 10, y + 40, 200, 20},
                                                     TextFormat("Price : %d", tower._cost));
                                            GuiLabel((Rectangle){x + 10, y + 70, 200, 20},
                                                     TextFormat("Range : %d", tower._range));

                                            if (GuiButton((Rectangle){x + 10, y + 100, 100, 30}, "Buy"))
                                            {
                                                // Do something
                                            }
                                            const Texture2D texture = *tower._texture;

                                            tower._frame_counter += m._game._frame_time;
                                            if (tower._frame_counter >= m._game._frame_time * 15)
                                            {
                                                tower._frame_counter = 0.0f;
                                                tower._frame++;

                                                if (tower._frame >= 3)
                                                {
                                                    tower._frame = 0;
                                                }
                                            }

                                            Rectangle sourceRect = {0.0f, static_cast<float>(tower._frame * 32), 32.0f,
                                                                    32.0f};
                                            Rectangle destRect = {x + 100, y + 10, sourceRect.width * scale,
                                                                  sourceRect.height * scale};
                                            DrawTexturePro(texture, sourceRect, destRect, {0, 0}, 0.0f, WHITE);
                                        }
                                    }
                                }
                            }

                            for (auto &enemy : m._enemies)
                            {
                                const Texture2D texture = *enemy._texture;

                                enemy._frame_counter += m._game._frame_time;
                                if (enemy._frame_counter >= m._game._frame_time * 15)
                                {
                                    enemy._frame_counter = 0.0f;
                                    enemy._frame++;

                                    if (enemy._frame >= 3)
                                    {
                                        enemy._frame = 0;
                                    }
                                }

                                Rectangle sourceRect = {0.0f, static_cast<float>(enemy._frame * 32), 32.0f,
                                                        32.0f};
                                Rectangle destRect = {enemy._pos_x * sourceRect.width * scale,
                                    enemy._pos_y * sourceRect.height * scale,
                                    sourceRect.width * scale,
                                    sourceRect.height * scale};

                                Vector2 origin = {destRect.width / 2.0f, destRect.height / 2.0f};
                                DrawTexturePro(texture, sourceRect, destRect, origin, 0.0f, WHITE);
                            }
                            // Check if wave is clear

                            for (auto& enemy : m._enemies)
                            {
                                if (enemy._current_step >= enemy._speed) // Change logic
                                {
                                    for (size_t j = 0; j < m._path.size(); j++)
                                    {
                                        if (m._path[j]._x == enemy._pos_x && m._path[j]._y == enemy._pos_y)
                                        {
                                            if (j + 1 < m._path.size())
                                            {
                                                enemy._pos_x = m._path[j + 1]._x;
                                                enemy._pos_y = m._path[j + 1]._y;
                                                spdlog::info("Enemy moved to ({}, {})", enemy._pos_x, enemy._pos_y);
                                            }
                                            enemy._current_step = 0;
                                            break;
                                        }
                                    }
                                }
                                else
                                {
                                    enemy._current_step += 1;
                                }
                            }

                            // Spawn
                            for (int i = 0; i < m._game._enemy_waves[m._game._current_wave].size(); i++)
                            {
                                if (m._game._enemy_waves[m._game._current_wave][i]._amount > 0)
                                {
                                    int start_x = m._path[0]._x;
                                    int start_y = m._path[0]._y;

                                    switch (m._game._enemy_waves[m._game._current_wave][i]._enemy_type)
                                    {
                                    case tower_defense::Basic_slime:
                                        m._enemies.emplace_back(EnemyComponent{1, 2000, 0, 1, 1,
                                            tm.get_texture(tower_defense::BASIC_SLIME), start_x, start_y, 0,
                                            0});
                                        break;
                                    case tower_defense::Bat:
                                        m._enemies.emplace_back(EnemyComponent{1, 1, 0, 1, 1,
                                            tm.get_texture(tower_defense::BAT), start_x, start_y, 0,
                                            0});
                                        break;
                                    case tower_defense::Zombie:
                                        m._enemies.emplace_back(EnemyComponent{1, 1, 0, 1, 1,
                                            tm.get_texture(tower_defense::ZOMBIE), start_x, start_y, 0,
                                            0});
                                        break;
                                    default:
                                        break;
                                    }
                                    m._game._enemy_waves[m._game._current_wave][i]._amount -= 1;
                                }
                            }
                            // Game logic
                        }
                    }
                    EndDrawing();
                }
            }
        }
    }
} // namespace ecs

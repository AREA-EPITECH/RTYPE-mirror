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
    void draw_game_system(Registry &ecs, const WindowDrawEvent &) {
        ecs.run_event(ControlsEvent{});
        auto &windows = ecs.get_components<Window>();
        auto &maps = ecs.get_components<MapComponent>();
        auto &selectors = ecs.get_components<SelectorComponent>();
        auto &shops = ecs.get_components<Shop>();

        SelectorComponent s = {};

        for (auto &selector : selectors) {
            if (selector.has_value()) {
                s = selector.value();
                break;
            }
        }

        for (auto &window : windows) {
            if (window.has_value()) {
                auto &win = window.value();
                if (win.isOpen) {
                    BeginDrawing();
                    ClearBackground(RAYWHITE);

                    for (auto &map : maps) {
                        if (map.has_value()) {
                            auto &m = map.value();
                            const int scale = 4;

                            for (auto &g : m._grass)
                            {
                                const Texture2D texture = *g._texture;
                                DrawTextureEx(texture, {static_cast<float>(texture.width * scale * g._x),
                                    static_cast<float>(texture.height * scale * g._y)},
                                    0, 4, WHITE);
                            }
                            for (auto &p : m._path)
                            {
                                const Texture2D texture = *p._texture;
                                DrawTextureEx(texture, {static_cast<float>(texture.width * scale * p._x),
                                    static_cast<float>(texture.height * scale * p._y)},
                                    0, 4, WHITE);
                            }
                            for (auto &d : m._decors)
                            {
                                const Texture2D texture = *d._texture;
                                DrawTextureEx(texture, {static_cast<float>(texture.width * scale * d._x),
                                    static_cast<float>(texture.height * scale * d._y)},
                                    0, 4, WHITE);
                            }

                            if (s._drawable)
                            {
                                DrawTextureEx(s._texture, {static_cast<float>(s._texture.width * scale * s._pos._x),
                                    static_cast<float>(s._texture.height * scale * s._pos._y)},
                                    0, 4, WHITE);
                            }

                            DrawText(m._game._map_name.c_str(), 10, GetScreenHeight() - 30, 32, RED);

                            DrawTextureEx(m._game._money._texture, {2,
                                static_cast<float>(GetScreenHeight() - 100 - m._game._money._texture.height)},
                                0, 4, WHITE);
                            DrawText(std::to_string(m._game._money._value).c_str(), 5 + m._game._money._texture.width * 2,
                                GetScreenHeight() - 50 - m._game._money._texture.height, 32, GOLD);

                            for (auto &shop: shops)
                            {
                                if (shop.has_value())
                                {
                                    auto &s = shop.value();
                                    if (GuiButton((Rectangle){ 24, 24, 120, 30 }, "Shop"))
                                    {
                                        s._open = !s._open;
                                    }
                                    if (s._open)
                                    {
                                        GuiLabel((Rectangle){ 10, 100, 200, 200 }, "Shop");
                                        for (int i = 0; i < s._towers.size(); i++)
                                        {
                                            const float x = static_cast<float>(20) + static_cast<float>(i % 3) * 260;
                                            const float y = static_cast<float>(60) + static_cast<float>(i / 3) * 160;

                                            GuiGroupBox((Rectangle){ static_cast<float>(x), static_cast<float>(y), 240, 140 }, "");

                                            GuiLabel((Rectangle){ x + 10, y + 10, 200, 20 }, s._towers[i]._name.c_str());
                                            GuiLabel((Rectangle){ x + 10, y + 40, 200, 20 }, TextFormat("Price : %d", s._towers[i]._cost));
                                            GuiLabel((Rectangle){ x + 10, y + 70, 200, 20 }, TextFormat("Range : %d", s._towers[i]._range));

                                            if (GuiButton((Rectangle){ x + 10, y + 100, 100, 30 }, "Buy")) {
                                                // Do something
                                            }
                                            const Texture2D texture = *s._towers[i]._texture;
                                            DrawTextureEx(texture, {x + 160, y + 10}, 0, 4, WHITE);
                                        }
                                    }

                                }
                            }

                        }
                    }
                    EndDrawing();
                }
            }
        }
    }
}

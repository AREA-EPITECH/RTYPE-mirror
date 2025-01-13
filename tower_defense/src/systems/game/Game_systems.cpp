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
                                DrawTextureEx(g._texture, {static_cast<float>(g._texture.width * scale * g._x),
                                    static_cast<float>(g._texture.height * scale * g._y)},
                                    0, 4, WHITE);
                            }
                            for (auto &p : m._path)
                            {
                                DrawTextureEx(p._texture, {static_cast<float>(p._texture.width * scale * p._x),
                                    static_cast<float>(p._texture.height * scale * p._y)},
                                    0, 4, WHITE);
                            }
                            for (auto &p : m._decors)
                            {
                                DrawTextureEx(p._texture, {static_cast<float>(p._texture.width * scale * p._x),
                                    static_cast<float>(p._texture.height * scale * p._y)},
                                    0, 4, WHITE);
                            }

                            DrawText(m._game._map_name.c_str(), 10, GetScreenHeight() - 30, 32, RED);

                            DrawTextureEx(m._game._money._texture, {2,
                                static_cast<float>(GetScreenHeight() - 100 - m._game._money._texture.height)},
                                0, 4, WHITE);
                            DrawText(std::to_string(m._game._money._value).c_str(), 5 + m._game._money._texture.width * 2,
                                GetScreenHeight() - 50 - m._game._money._texture.height, 32, GOLD);

                            if (s._drawable)
                            {
                                DrawTextureEx(s._texture, {static_cast<float>(s._texture.width * scale * s._pos._x),
                                    static_cast<float>(s._texture.height * scale * s._pos._y)},
                                    0, 4, WHITE);
                            }
                        }
                    }
                    EndDrawing();
                }
            }
        }
    }
}

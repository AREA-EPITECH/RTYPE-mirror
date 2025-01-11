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
        auto &windows = ecs.get_components<Window>();
        auto &maps = ecs.get_components<MapComponent>();

        for (auto &window : windows) {
            if (window.has_value()) {
                auto &win = window.value();
                if (win.isOpen) {
                    BeginDrawing();
                    ClearBackground(RAYWHITE);

                    for (auto &map : maps) {
                        if (map.has_value()) {
                            const int scale = 4;

                            auto &m = map.value();
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
                        }
                    }
                    EndDrawing();
                }
            }
        }
    }
}

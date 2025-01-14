/*
** EPITECH PROJECT, 2024
** r-type
** File description:
** Controls_systems
*/

#include "ecs/Systems.hpp"

namespace ecs
{
    /**
     * Get controls in the game
     * @param ecs
     */
    void game_controls_system(Registry &ecs, const ControlsEvent &)
    {
        auto &selectors = ecs.get_components<ecs::SelectorComponent>();

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            for (auto &selector: selectors)
            {
                if (selector.has_value())
                {
                    auto &s = selector.value();

                    int scale = 4;

                    Vector2 mouse_pos =  GetMousePosition();
                    if (s._pos._x == static_cast<int>(mouse_pos.x) / (32 * scale) &&
                        s._pos._y == static_cast<int>(mouse_pos.y) / (32 * scale) && s._drawable)
                    {
                        s._drawable = false;
                    } else
                    {
                        s._pos._x = static_cast<int>(mouse_pos.x) / (32 * scale);
                        s._pos._y = static_cast<int>(mouse_pos.y) / (32 * scale);
                        s._drawable = true;
                    }
                }
            }
        }
    }
} // namespace ecs

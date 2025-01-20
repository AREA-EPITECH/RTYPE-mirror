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
        auto &selectors = ecs.get_components<SelectorComponent>();
        auto &shops = ecs.get_components<Shop>();

        Shop shop = {};

        for (auto &s : shops)
        {
            if (s.has_value())
            {
                shop = s.value();
                break;
            }
        }

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            for (auto &selector : selectors)
            {
                if (selector.has_value())
                {
                    if (shop._open)
                    {
                        break;
                    }
                    auto &s = selector.value();
                    int scale = 4;
                    Vector2 mouse_pos = GetMousePosition();
                    bool is_click_in_no_clickable = false;

                    for (int i = 0; i < selector->_no_clickable.size(); i++)
                    {
                        if (CheckCollisionPointRec(mouse_pos, selector->_no_clickable[i]))
                        {
                            is_click_in_no_clickable = true;
                            break;
                        }
                    }
                    if (is_click_in_no_clickable)
                    {
                        break;
                    }

                    if (s._position.x == static_cast<int>(mouse_pos.x) / (32 * scale) &&
                        s._position.y == static_cast<int>(mouse_pos.y) / (32 * scale) && s._drawable)
                    {
                        s._drawable = false;
                    }
                    else
                    {
                        s._position.x = static_cast<int>(mouse_pos.x) / (32 * scale);
                        s._position.y = static_cast<int>(mouse_pos.y) / (32 * scale);
                        s._drawable = true;
                    }
                }
            }
        }
    }
} // namespace ecs

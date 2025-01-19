/*
** EPITECH PROJECT, 2024
** r-type
** File description:
** Systems
*/

#pragma once

#include <algorithm>
#include "Events.hpp"
#include "Components.hpp"
#include "Registry.hpp"
#include "Main.hpp"

namespace ecs {
    // Window
    void init_window_system(Registry &ecs, const CreateWindowEvent &);
    void close_window_system(Registry &ecs, const WindowCloseEvent &);

    // Game
    void draw_game_system(Registry &ecs, const WindowDrawEvent &);

    // Controls
    void game_controls_system(Registry &ecs, const ControlsEvent &);
}

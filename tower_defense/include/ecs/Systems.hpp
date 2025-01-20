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
    void init_register_system(Registry &ecs, const InitRegisterEvent &);
    void close_window_system(Registry &ecs, const WindowCloseEvent &);

    // Game
    void draw_game_system(Registry &ecs, const WindowDrawEvent &);

    // Controls
    void game_controls_system(Registry &ecs, const ControlsEvent &);

    // Text
    void create_text_component(Registry &ecs, const CreateTextEvent &);

    // Tower
    void create_tower_component(Registry &ecs, const CreateTowerEvent &);
}

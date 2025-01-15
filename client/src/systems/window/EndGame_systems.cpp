/*
** EPITECH PROJECT, 2024
** r-type
** File description:
** End_game_systems
*/

#include "ecs/Systems.hpp"

namespace ecs {
    void draw_endgame_system(Registry &ecs, const WindowDrawEvent &) {
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLUE, 0.9f));
    }

    void open_endgame_system(Registry &ecs, const WindowOpenEvent &) {

    }

    void close_endgame_system(Registry &ecs, const WindowCloseEvent &) {

    }
}
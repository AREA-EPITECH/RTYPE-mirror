/*
** EPITECH PROJECT, 2024
** r-type
** File description:
** End_game_systems
*/

#include "ecs/Systems.hpp"

namespace ecs {
    void draw_endgame_system(Registry &ecs, const WindowDrawEvent &) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLUE, 0.9f));
        EndDrawing();
    }

    void open_endgame_system(Registry &ecs, const WindowOpenEvent &) {
    }

    void close_endgame_system(Registry &ecs, const WindowCloseEvent &) {
    }
}
/*
** EPITECH PROJECT, 2024
** r-type
** File description:
** Game_systems
*/

#include "ecs/Systems.hpp"

namespace ecs {
    bool check_endgame (Registry &ecs) {
        auto &scores = ecs.get_components<ScoreComponent>();

        for (auto &score : scores) {
            if (score.has_value()) {
                score.value().draw_ingame();
                /*if (score.value().score >= score.value().win_score) {
                    EndDrawing();
                    change_window(ecs, END_GAME);
                    return true;
                }*/
            }
        }
        return false;
    }
}
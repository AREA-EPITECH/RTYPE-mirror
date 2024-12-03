//
// Created by lferraro on 12/3/24.
//

#include "ecs/Systems.hpp"

namespace ecs {

    void draw_game_system(Registry &ecs, const WindowDrawEvent &) {
        ecs.run_event(ControlsEvent{});
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("Je suis dans le jeu", 10, 10, 20, DARKGRAY);
        EndDrawing();

        if (WindowShouldClose()) {
            ecs.run_event(WindowCloseEvent{true});
        }
    }

}
//
// Created by lferraro on 12/2/24.
//

#include "include/Systems.hpp"

void draw_menu_system(Registry& ecs, const WindowDrawEvent&) {
    ecs.run_event(ControlsEvent{});
    BeginDrawing();
    ClearBackground(RAYWHITE);

    auto& cameras = ecs.get_components<CameraComponent>();
    for (std::size_t i = 0; i < cameras.size(); ++i) {
        if (cameras[i].has_value()) {
            Camera& camera = cameras[i]->camera;

            BeginMode3D(camera);

            auto& models = ecs.get_components<ModelComponent>();
            for (std::size_t j = 0; j < models.size(); ++j) {
                if (models[j].has_value()) {
                    const Model& model = models[j].value().model;
                    if ( models[j].value().drawable)
                        DrawModel(model, {0, 0, 0}, 1.0f, WHITE);
                }
            }

            EndMode3D();
            break;
        }
    }

    DrawText("Hello ECS with Raylib!", 10, 10, 20, DARKGRAY);

    EndDrawing();

    if (WindowShouldClose()) {
        ecs.run_event(WindowCloseEvent{});
    }
}
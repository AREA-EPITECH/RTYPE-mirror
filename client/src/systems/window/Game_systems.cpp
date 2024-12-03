//
// Created by lferraro on 12/3/24.
//

#include "ecs/Systems.hpp"

namespace ecs {

    void draw_game_system(Registry &ecs, const WindowDrawEvent &) {
        ecs.run_event(ControlsEvent{});
        BeginDrawing();
        ClearBackground(RAYWHITE);

        auto &cameras = ecs.get_components<CameraComponent>();
        static float rotationAngle = 0.01f;

        for (auto & i : cameras) {
            if (i.has_value()) {
                Camera &camera = i->camera;

                BeginMode3D(camera);

                auto &models = ecs.get_components<ModelComponent>();
                for (auto & model : models) {
                    if (model.has_value()) {
                        ModelComponent &modelComponent = model.value();
                        if (modelComponent.drawable) {
                            Matrix rotation = MatrixRotate((Vector3){0.0f, 1.0f, 0.0f}, rotationAngle);
                            modelComponent.model.transform = MatrixMultiply(modelComponent.model.transform, rotation);
                            DrawModel(modelComponent.model, {0, 0, 0}, 1.0f, WHITE);
                        }
                    }
                }

                EndMode3D();
                break;
            }
        }

        DrawText("In Game !", 10, 10, 20, DARKGRAY);

        EndDrawing();

        if (WindowShouldClose()) {
            ecs.run_event(WindowCloseEvent{});
        }
    }

    void open_game_system(Registry &ecs, const WindowOpenEvent &) {
        ecs.run_event(InitCameraEvent{{0.0f, 20.0f, 20.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, 45.0f,
                                      CAMERA_PERSPECTIVE});
        ecs.run_event(InitModelEvent{});
    }

    void close_game_system(Registry &ecs, const WindowCloseEvent &) {
        auto &models = ecs.get_components<ModelComponent>();

        for (std::size_t i = 0; i < models.size(); ++i) {
            if (models[i].has_value()) {
                UnloadModel(models[i]->model);
                TraceLog(LOG_WARNING, TextFormat("Unloaded model for entity %zu.", i));

                ecs.kill_entity(static_cast<entity_t>(i));
            }
        }
    }

}
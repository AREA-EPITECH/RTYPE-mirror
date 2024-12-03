//
// Created by lferraro on 12/2/24.
//
#include "ecs/Systems.hpp"
namespace ecs {
    void draw_lobby_system(Registry &ecs, const WindowDrawEvent &) {
        ecs.run_event(ControlsEvent{});
        BeginDrawing();
        ClearBackground(RAYWHITE);

        auto &cameras = ecs.get_components<CameraComponent>();
        for (std::size_t i = 0; i < cameras.size(); ++i) {
            if (cameras[i].has_value()) {
                Camera &camera = cameras[i]->camera;

                BeginMode3D(camera);

                auto &models = ecs.get_components<ModelComponent>();
                for (std::size_t j = 0; j < models.size(); ++j) {
                    if (models[j].has_value()) {
                        const Model &model = models[j].value().model;
                        if (models[j].value().drawable)
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

    void open_lobby_system(Registry &ecs, const WindowOpenEvent &) {
        ecs.run_event(InitCameraEvent{{0.0f, 25.0f, 10.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, 45.0f,
                                      CAMERA_PERSPECTIVE});
        ecs.run_event(InitModelEvent{});
    }

    void close_lobby_system(Registry &ecs, const WindowCloseEvent &) {
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
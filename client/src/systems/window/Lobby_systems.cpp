/*
** EPITECH PROJECT, 2024
** r-type
** File description:
** Lobby_systems
*/

#include "ecs/Systems.hpp"

namespace ecs {

    /**
     * Draw the lobby window
     * @param ecs
     */
    void draw_lobby_system(Registry &ecs, const WindowDrawEvent &) {
        ecs.run_event(ControlsEvent{});
        BeginDrawing();
        ClearBackground(RAYWHITE);

        const int screenWidth = GetScreenWidth();
        const int screenHeight = GetScreenHeight();

        update_board_component(ecs, screenWidth, screenHeight);

        auto &cameras = ecs.get_components<CameraComponent>();
        static float rotationAngle = 0.01f;

        display_board(ecs, screenWidth, screenHeight);

        for (auto &i : cameras) {
            if (i.has_value()) {
                Camera &camera = i->camera;

                BeginMode3D(camera);

                auto &models = ecs.get_components<VesselsComponent>();
                for (auto &model : models) {
                    if (model.has_value()) {
                        VesselsComponent &modelComponent = model.value();
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

        EndDrawing();

        if (WindowShouldClose()) {
            ecs.run_event(WindowCloseEvent{});
        }
    }

    /**
     * Open the lobby window
     * @param ecs
     */
    void open_lobby_system(Registry &ecs, const WindowOpenEvent &) {
        ecs.run_event(InitCameraEvent{{10.0f, 10.0f, 30.0f},
                                      {-10.0f, 0.0f, 0.0f},
                                      {0.0f, 30.0f, 0.0f},
                                      45.0f,
                                      CAMERA_PERSPECTIVE});
        ecs.run_event(InitModelEvent{});
        ecs.run_event(InitShaderEvent{"client/assets/voxels/shaders/voxel_lighting.vs",
                                      "client/assets/voxels/shaders/voxel_lighting.fs"});
    }

    /**
     * Close the lobby window
     * @param ecs
     */
    void close_lobby_system(Registry &ecs, const WindowCloseEvent &) {
        auto &models = ecs.get_components<VesselsComponent>();
        auto &camera = ecs.get_components<CameraComponent>();
        auto &texts = ecs.get_components<TextComponent>();
        auto &buttons = ecs.get_components<ButtonComponent>();

        for (std::size_t i = 0; i < models.size(); ++i) {
            if (models[i].has_value()) {
                if (!models[i].value().drawable) {
                    UnloadModel(models[i]->model);
                    TraceLog(LOG_INFO, TextFormat("Unloaded model for entity %zu.", i));
                    ecs.kill_entity(i);
                }
            }
        }
        for (std::size_t i = 0; i < camera.size(); ++i) {
            if (camera[i].has_value()) {
                ecs.kill_entity(i);
            }
        }
        for (std::size_t i = 0; i < texts.size(); ++i) {
            if (texts[i].has_value()) {
                ecs.kill_entity(i);
            }
        }
        for (std::size_t i = 0; i < buttons.size(); ++i) {
            if (buttons[i].has_value()) {
                ecs.kill_entity(i);
            }
        }
    }
}
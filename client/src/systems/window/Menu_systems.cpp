//
// Created by lferraro on 12/2/24.
//

#include "ecs/Systems.hpp"

namespace ecs {
    /**
 * Draw the menu window
 * @param ecs
 */
void draw_menu_system(Registry &ecs, const WindowDrawEvent &) {
    ecs.run_event(ControlsEvent{});
    BeginDrawing();
    ClearBackground(RAYWHITE);

    const int screenWidth = GetScreenWidth();
    const int screenHeight = GetScreenHeight();
    update_menu_selectors(ecs, screenWidth, screenHeight);
    display_menu_selectors(ecs);
    auto &cameras = ecs.get_components<CameraComponent>();

    for (auto &camera_i: cameras) {
        if (camera_i.has_value()) {
            Camera &camera = camera_i->camera;

            BeginMode3D(camera);

            auto &models = ecs.get_components<MenuText>();
            for (auto &model: models) {
                if (model.has_value()) {
                    MenuText &modelComponent = model.value();
                    DrawModel(modelComponent.model, {0, 0, 0}, 1.5f, WHITE);
                }
            }

            EndMode3D();
            break;
        }
    }
        EndDrawing();

        if (WindowShouldClose()) {
            ecs.run_event(WindowCloseEvent{true});
        }
    }

    void open_menu_system(Registry &ecs, const WindowOpenEvent &) {
        ecs.run_event(InitCameraEvent{{40.0f, -10.0f, 0.0f}, {0.0f, -10.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, 45.0f,
                                      CAMERA_PERSPECTIVE});
        ecs.run_event(InitModelEvent{});
    }

    void close_menu_system(Registry &ecs, const WindowCloseEvent &) {
        auto &models = ecs.get_components<MenuText>();

        for (std::size_t i = 0; i < models.size(); ++i) {
            if (models[i].has_value()) {
                UnloadModel(models[i]->model);
                TraceLog(LOG_INFO, TextFormat("Unloaded model for entity %zu.", i));
                ecs.kill_entity(i);
            }
        }

        kill_entities_with_component<CameraComponent>(ecs);
        kill_entities_with_component<ButtonComponent>(ecs);
        kill_entities_with_component<TextInputComponent>(ecs);
        kill_entities_with_component<ShowBoxComponent>(ecs);
    }
}
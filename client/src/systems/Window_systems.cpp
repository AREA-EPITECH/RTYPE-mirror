//
// Created by lferraro on 12/2/24.
//
#include "Systems.hpp"

void init_window_system(Registry& ecs, const WindowOpenEvent&) {
    auto& windows = ecs.get_components<Window>();
    for (size_t i = 0; i < windows.size(); ++i) {
        if (windows[i]) {
            auto& win = windows[i].value();
            if (!win.isOpen) {
                TraceLog(LOG_WARNING, TextFormat("Initiating window %dx%d named %s.", win.width, win.height, win.title.c_str()));
                SetConfigFlags(FLAG_WINDOW_RESIZABLE);
                InitWindow(win.width, win.height, win.title.c_str());
                SetTargetFPS(144);
                win.isOpen = true;
                break;
            }
        }
    }
}

void close_window_system(Registry& ecs, const WindowCloseEvent&) {
    auto& windows = ecs.get_components<Window>();
    for (size_t i = 0; i < windows.size(); ++i) {
        if (windows[i]) {
            auto& win = windows[i].value();
            if (win.isOpen) {
                CloseWindow();
                win.isOpen = false;
                break;
            }
        }
    }
}

void update_window_system(Registry& ecs, const WindowUpdateEvent&) {
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
                    const Model& model = models[j]->model;
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
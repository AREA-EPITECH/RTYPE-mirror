/*
** EPITECH PROJECT, 2024
** r-type
** File description:
** Menu_systems
*/
#include "ecs/Systems.hpp"

namespace ecs {
    /**
 * Draw the menu window
 * @param ecs
 */
void draw_menu_system(Registry &ecs, const WindowDrawEvent &) {
    ecs.run_event(ControlsEvent{});
    auto &backgrounds = ecs.get_components<BackgroundComponent>();
    auto &decors = ecs.get_components<DecorElementComponent>();

    for (auto & background : backgrounds) {
        if (background.has_value()) {
            background->offset -= background->speed * GetFrameTime();
            float textureWidthOnScreen = static_cast<float>(background->texture.width) *
                (static_cast<float>(GetScreenHeight()) / static_cast<float>(background->texture.height));
            if (background->offset <= -textureWidthOnScreen) {
                background->offset = 0;
            }
        }
    }
    float deltaTime = GetFrameTime();
    for (auto &decor : decors) {
        if (decor.has_value()) {
            decor->Update(deltaTime, GetScreenWidth(), GetScreenHeight());
        }
    }

    BeginDrawing();
    ClearBackground(RAYWHITE);

    for (auto & background : backgrounds) {
        if (background.has_value()) {
            background->DrawLayer(GetScreenWidth(), GetScreenHeight());
        }
    }
    for (auto &decor : decors) {
        if (decor.has_value()) {
            decor->DrawDecorElement(GetScreenWidth(), GetScreenHeight());
        }
    }

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
        auto &cameras = ecs.get_components<CameraComponent>();
        Camera camera = {};
        for (auto & camera_i : cameras) {
            if (camera_i.has_value()) {
                camera = camera_i->camera;
                break;
            }
        }
        ecs.run_event(InitLightEvent{client::LIGHT_DIRECTIONAL, {0, 0, 0},
            Vector3Normalize(Vector3Subtract(camera.target, camera.position)), WHITE, 0});

        ecs.run_event(InitModelEvent{});
        ecs.run_event(InitShaderEvent{"client/assets/voxels/shaders/voxel_lighting.vs",
                                  "client/assets/voxels/shaders/voxel_lighting.fs"});
        auto &shaders = ecs.get_components<ShaderComponent>();
        Shader shader = {};
        for (auto & shader_i : shaders) {
            if (shader_i.has_value()) {
                shader = shader_i->shader;
                break;
            }
        }
        auto &lights = ecs.get_components<LightComponent>();
        for (auto & light : lights) {
            if (light.has_value()) {
                light->light->UpdateLightValues(shader);
            }
        }
        // Init background
        ecs.run_event(InitBackgroundEvent{"client/assets/backgrounds/game/space_background.png", 2,
                    200, 0});
        ecs.run_event(InitDecorElementEvent{"client/assets/backgrounds/game/space_midground.png", 300});
        ecs.run_event(InitDecorElementEvent{"client/assets/backgrounds/game/space_midground_2.png", 300});
        ecs.run_event(InitDecorElementEvent{"client/assets/backgrounds/game/space_foreground.png", 400});
    }

    void close_menu_system(Registry &ecs, const WindowCloseEvent &) {
        auto &models = ecs.get_components<MenuText>();
        auto &backgrounds = ecs.get_components<BackgroundComponent>();
        auto &decors = ecs.get_components<DecorElementComponent>();

        for (std::size_t i = 0; i < models.size(); ++i) {
            if (models[i].has_value()) {
                TraceLog(LOG_INFO, TextFormat("Unloaded model for entity %zu.", i));
                ecs.kill_entity(i);
            }
        }
        for (std::size_t i = 0; i < backgrounds.size(); ++i) {
            if (backgrounds[i].has_value()) {
                UnloadTexture(backgrounds[i]->texture);
                ecs.kill_entity(i);
            }
        }
        for (std::size_t i = 0; i < decors.size(); ++i) {
            if (decors[i].has_value()) {
                UnloadTexture(decors[i]->texture);
                ecs.kill_entity(i);
            }
        }
        kill_entities_with_component<LightComponent>(ecs);
        kill_entities_with_component<CameraComponent>(ecs);
        kill_entities_with_component<ButtonComponent>(ecs);
        kill_entities_with_component<TextInputComponent>(ecs);
        kill_entities_with_component<ShowBoxComponent>(ecs);
    }
}
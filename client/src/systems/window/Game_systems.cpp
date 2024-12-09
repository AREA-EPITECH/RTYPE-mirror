/*
** EPITECH PROJECT, 2024
** r-type
** File description:
** Game_systems
*/

#include "ecs/Systems.hpp"

namespace ecs {

    /**
     * Draw the game window
     * @param ecs
     */
    void draw_game_system(Registry &ecs, const WindowDrawEvent &) {
        ecs.run_event(ControlsEvent{});
        auto &shaders = ecs.get_components<ShaderComponent>();
        auto &backgrounds = ecs.get_components<BackgroundComponent>();
        auto &decors = ecs.get_components<DecorElementComponent>();
        Shader shader = {};
        for (auto & shader_i : shaders) {
            if (shader_i.has_value()) {
                shader = shader_i->shader;
                break;
            }
        }

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

        auto &cameras = ecs.get_components<CameraComponent>();

        for (auto & camera_i : cameras) {
            if (camera_i.has_value()) {
                Camera &camera = camera_i->camera;

                auto &lights = ecs.get_components<LightComponent>();
                for (auto & light : lights) {
                    if (light.has_value()) {
                        light->light->UpdateLightValues(shader);
                    }
                }

                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                {
                    ecs.run_event(ParticleSystemEvent{{GetMousePosition().x, GetMousePosition().y, 0},
                        client::UP, true, 100, 1000});
                }

                auto &particles_systems = ecs.get_components<ParticleSystemComponent>();
                for (size_t i = 0; i < particles_systems.size(); ++i) {
                    if (particles_systems[i].has_value()) {
                        ParticleSystemComponent &particleSystemComponent = particles_systems[i].value();
                        if (!particleSystemComponent.particleSystem.isAlive())
                        {
                            ecs.kill_entity(i);
                        } else
                        {
                            particleSystemComponent.particleSystem.update();
                            particleSystemComponent.particleSystem.draw();
                        }
                    }
                }

                BeginMode3D(camera);

                auto &models = ecs.get_components<VesselsComponent>();
                for (auto & model : models) {
                    if (model.has_value()) {
                        VesselsComponent &modelComponent = model.value();
                        if (modelComponent.drawable) {
                            DrawModel(modelComponent.model, {0, 0, 0}, 1.0f, WHITE);
                        }
                    }
                }

                for (auto & light : lights) {
                    if (light.has_value()) {
                        if (light->light->_enabled)
                        {
                            DrawSphereEx(light->light->_position, 0.2f, 8, 8, light->light->_color);
                        }
                        else
                        {
                            DrawSphereWires(light->light->_position, 0.2f, 8, 8,
                            ColorAlpha(light->light->_color, 0.3f));
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


    /**
    * Open the game window
    * @param ecs
    */
    void open_game_system(Registry &ecs, const WindowOpenEvent &) {
        ecs.run_event(InitCameraEvent{{0.0f, 20.0f, 20.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, 45.0f,
                                      CAMERA_PERSPECTIVE});
        ecs.run_event(InitModelEvent{});
        ecs.run_event(InitShaderEvent{});
        auto &shaders = ecs.get_components<ShaderComponent>();
        Shader shader = {};
        for (auto & shader_i : shaders) {
            if (shader_i.has_value()) {
                shader = shader_i->shader;
                break;
            }
        }
        ecs.run_event(InitLightEvent{client::LIGHT_POINT, {-20, 20, -20}, Vector3Zero(),
            WHITE, shader, 0});
        ecs.run_event(InitLightEvent{client::LIGHT_POINT, {20, -20, 20}, Vector3Zero(),
            WHITE, shader, 1});
        ecs.run_event(InitLightEvent{client::LIGHT_POINT, {-20, 20, 20}, Vector3Zero(),
            WHITE, shader, 2});
        ecs.run_event(InitLightEvent{client::LIGHT_POINT, {20, -20, -20}, Vector3Zero(),
            WHITE, shader, 3});

        ecs.run_event(InitBackgroundEvent{"client/assets/backgrounds/game/space_background.png", 2,
                    500, 0});
        ecs.run_event(InitDecorElementEvent{"client/assets/backgrounds/game/space_midground.png"});
        ecs.run_event(InitDecorElementEvent{"client/assets/backgrounds/game/space_midground_2.png"});
        ecs.run_event(InitDecorElementEvent{"client/assets/backgrounds/game/space_foreground.png"});

    }

    /**
    * Close the game window
    * @param ecs
    */
    void close_game_system(Registry &ecs, const WindowCloseEvent &) {
        auto &models = ecs.get_components<VesselsComponent>();
        auto &camera = ecs.get_components<CameraComponent>();
        auto &particles_systems = ecs.get_components<ParticleSystemComponent>();
        auto &lights = ecs.get_components<LightComponent>();
        auto &shaders = ecs.get_components<ShaderComponent>();
        auto &backgrounds = ecs.get_components<BackgroundComponent>();
        auto &decors = ecs.get_components<DecorElementComponent>();

        for (std::size_t i = 0; i < models.size(); ++i) {
            if (models[i].has_value()) {
                UnloadModel(models[i]->model);
                TraceLog(LOG_WARNING, TextFormat("Unloaded model for entity %zu.", i));
                ecs.kill_entity(i);
            }
        }
        for (std::size_t i = 0; i < camera.size(); ++i) {
            if (camera[i].has_value()) {
                ecs.kill_entity(i);
            }
        }
        for (size_t i = 0; i < particles_systems.size(); ++i) {
            if (particles_systems[i].has_value()) {
                TraceLog(LOG_WARNING, TextFormat("Unloaded particles system for entity %zu.", i));
                ecs.kill_entity(i);
            }
        }
        for (size_t i = 0; i < lights.size(); ++i) {
            if (lights[i].has_value()) {
                TraceLog(LOG_WARNING, TextFormat("Unloaded light for entity %zu.", i));
                ecs.kill_entity(i);
            }
        }
        for (std::size_t i = 0; i < shaders.size(); ++i) {
            if (shaders[i].has_value()) {
                UnloadShader(shaders[i]->shader);
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
    }

}
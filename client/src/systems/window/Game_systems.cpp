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

                VesselsComponent* vessel_c = nullptr;
                auto &vessels = ecs.get_components<VesselsComponent>();
                for (auto & vessel : vessels) {
                    if (vessel.has_value())
                    {
                        if (vessel->drawable) {
                            vessel_c = &vessel.value();
                            BoundingBox bbox = GetModelBoundingBox(vessel->model);
                            float x = bbox.max.x - bbox.min.x;
                            float y = bbox.max.y - bbox.min.y;
                            // ecs.run_event(ParticleSystemEvent{{GetWorldToScreen(vessel->position, camera).x - x,
                                // GetWorldToScreen(vessel->position, camera).y - y, 0}, client::LEFT, false,
                                // 5, 1000, RED});
                        }
                    }
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


                auto &projectiles = ecs.get_components<ProjectilesComponent>();
                for (auto & projectile : projectiles) {
                    if (projectile.has_value()) {
                        if (projectile->drawable) {
                            DrawModel(projectile->model, projectile->position, 1.0f, WHITE);
                        }
                    }
                }

                auto &models = ecs.get_components<VesselsComponent>();
                for (auto & model : models) {
                    if (model.has_value()) {
                        VesselsComponent &modelComponent = model.value();
                        if (modelComponent.drawable) {
                            DrawModel(modelComponent.model, modelComponent.position, 1.0f, WHITE);
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

        DrawText(TextFormat("FPS: %d", GetFPS()), 10, 10, 20, DARKGRAY);

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
        // Init camera
        ecs.run_event(InitCameraEvent{{0.0f, 0.0f, 50.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, 45.0f,
                                      CAMERA_PERSPECTIVE});
        // Init lights
        ecs.run_event(InitLightEvent{client::LIGHT_POINT, {-20, 20, -20}, Vector3Zero(),
            WHITE, 0});
        ecs.run_event(InitLightEvent{client::LIGHT_POINT, {20, -20, 20}, Vector3Zero(),
            WHITE, 1});
        ecs.run_event(InitLightEvent{client::LIGHT_POINT, {-20, 20, 20}, Vector3Zero(),
            WHITE, 2});
        ecs.run_event(InitLightEvent{client::LIGHT_POINT, {20, -20, -20}, Vector3Zero(),
            WHITE, 3});

        // Init models & shaders
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
        auto &lights = ecs.get_components<LightComponent>();
        for (auto & light : lights) {
            if (light.has_value()) {
                light->light->UpdateLightValues(shader);
            }
        }

        // Init
        ecs.run_event(InitBackgroundEvent{"client/assets/backgrounds/game/space_background.png", 2,
                    200, 0});
        ecs.run_event(InitDecorElementEvent{"client/assets/backgrounds/game/space_midground.png", 300});
        ecs.run_event(InitDecorElementEvent{"client/assets/backgrounds/game/space_midground_2.png", 300});
        ecs.run_event(InitDecorElementEvent{"client/assets/backgrounds/game/space_foreground.png", 400});
    }

    /**
    * Close the game window
    * @param ecs
    */
    void close_game_system(Registry &ecs, const WindowCloseEvent &) {
        auto &vessels_models = ecs.get_components<VesselsComponent>();
        auto &projectiles_models = ecs.get_components<ProjectilesComponent>();
        auto &camera = ecs.get_components<CameraComponent>();
        auto &particles_systems = ecs.get_components<ParticleSystemComponent>();
        auto &lights = ecs.get_components<LightComponent>();
        auto &shaders = ecs.get_components<ShaderComponent>();
        auto &backgrounds = ecs.get_components<BackgroundComponent>();
        auto &decors = ecs.get_components<DecorElementComponent>();

        for (std::size_t i = 0; i < vessels_models.size(); ++i) {
            if (vessels_models[i].has_value()) {
                UnloadModel(vessels_models[i]->model);
                TraceLog(LOG_WARNING, TextFormat("Unloaded model for entity %zu.", i));
                ecs.kill_entity(i);
            }
        }
        for (std::size_t i = 0; i < projectiles_models.size(); ++i) {
            if (projectiles_models[i].has_value() && !projectiles_models[i]->drawable) {
                UnloadModel(projectiles_models[i]->model);
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
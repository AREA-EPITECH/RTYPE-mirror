/*
** EPITECH PROJECT, 2024
** r-type
** File description:
** Game_systems
*/

#include "ecs/Systems.hpp"
#include <algorithm>

namespace ecs {

    /**
     * Draw the game window
     * @param ecs
     */
    void draw_game_system(Registry &ecs, const WindowDrawEvent &) {
        ecs.run_event(ControlsEvent{});
        auto &backgrounds = ecs.get_components<BackgroundComponent>();
        auto &shaders = ecs.get_components<ShaderComponent>();
        auto &decors = ecs.get_components<DecorElementComponent>();
        auto &health_bars = ecs.get_components<HealthBarComponent>();
        auto &vessels = ecs.get_components<VesselsComponent>();
        auto &cameras = ecs.get_components<CameraComponent>();
        auto &musics = ecs.get_components<MusicComponent>();

        for (auto &music : musics) {
            if (music.has_value()) {
                music.value().update("game_music");
            }
        }

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

        std::vector<Vector2> vessels_positions = {};
        std::vector<int> vessels_health = {};
        for (auto & vessel : vessels)
        {
            if (vessel.has_value())
            {
                for (auto & camera_i : cameras)
                {
                    if (camera_i.has_value())
                    {
                        VesselsComponent &vesselComponent = vessel.value();
                        if (vesselComponent.drawable)
                        {
                            vessels_positions.emplace_back(GetWorldToScreen(vesselComponent.position, camera_i->camera));
                            vessels_health.emplace_back(vesselComponent.health);
                        }
                    }
                }
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

        for (auto &health_bar: health_bars)
        {
            if (health_bar.has_value())
            {
                for (size_t i = 0; i < vessels_positions.size(); i++)
                {
                    DrawTexture(health_bar->textures[vessels_health[i]], static_cast<int>(vessels_positions[i].x),
                        static_cast<int>(vessels_positions[i].y), WHITE);
                }
            }
        }

        for (auto & camera_i : cameras) {
            if (camera_i.has_value()) {
                Camera &camera = camera_i->camera;

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
                for (size_t i = 0; i < projectiles.size(); ++i) {
                    if (projectiles[i].has_value()) {
                        ProjectilesComponent *projectile = &projectiles[i].value();
                        if (projectile->drawable) {
                            projectile->ApplyVelocity();
                            if (projectile->IsAlive(camera))
                            {
                                DrawModel(projectile->model, projectile->position, 1.0f, WHITE);
                                projectile->light->UpdateLightValues(shader);
                            }
                            else
                            {
                                projectile->light->UpdateLightValues(shader, false);
                                ecs.kill_entity(i);
                            }
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
                EndMode3D();
                break;
            }
        }

        DrawText(TextFormat("FPS: %d", GetFPS()), 10, 10, 20, DARKGRAY);

        auto &images = ecs.get_components<ecs::ImageComponent>();
        auto &settings = ecs.get_components<ecs::SettingsComponent>();

        for (auto & image : images) {
            if (image.has_value()) {
                image.value().draw(GetScreenWidth(), GetScreenHeight());
            }
        }

        for (auto & setting : settings) {
            if (setting.has_value()) {
                setting.value().background.width = (float)GetScreenWidth();
                setting.value().background.height = (float)GetScreenHeight();
                DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.9f));
                ecs.run_event(ecs::DisplaySettingEvent{});
            }
        }


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
        // Update camera
        auto &cameras = ecs.get_components<CameraComponent>();
        Camera camera = {};
        for (auto & camera_i : cameras) {
            if (camera_i.has_value()) {
                camera_i->camera.position = {0.0f, 0.0f, 50.0f};
                camera_i->camera.target = Vector3Zero();
                camera_i->camera.up = {0.0f, 1.0f, 0.0f};
                camera_i->camera.fovy = 45.0f;
                camera_i->camera.projection = CAMERA_PERSPECTIVE;
                camera = camera_i->camera;
                break;
            }
        }

        // Init lights
        ecs.run_event(InitLightEvent{client::LIGHT_DIRECTIONAL, {0, 0, 0},
            Vector3Normalize(Vector3Subtract(camera.target, camera.position)), WHITE, 0});

        // Init models & shaders
        ecs.run_event(InitModelEvent{});
        ecs.run_event(InitShaderEvent{});

        // Init health bars
        ecs.run_event(HealthBarEvent{"./client/assets/health_bar"});

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

        auto &backgrounds = ecs.get_components<BackgroundComponent>();
        for (auto & background : backgrounds) {
            if (background.has_value()) {
                background->speed = 200;
            }
        }

        auto &decors = ecs.get_components<DecorElementComponent>();
        for (auto & decor : decors) {
            if (decor.has_value()) {
                switch (decor->depth)
                {
                    case 1:
                        decor->speed = 100;
                        break;
                    case 2:
                        decor->speed = 200;
                        break;
                    case 3:
                        decor->speed = 300;
                        break;
                    default:
                        break;
                }
            }
        }

        auto settingsIcons = ecs.spawn_entity();
        std::string icon_path = ASSET_FILE("images/settings.png");
        float width = 100.0f;
        float height = 100.0f;

        ecs.add_component<ecs::ImageComponent>(settingsIcons, {icon_path, GAME_FOCUS,
                                                               [width](int screenWidth, int screenHeight) { return screenWidth - 50 - width; },
                                                               [](int screenWidth, int screenHeight) { return  50; },
                                                               width, height, [&ecs]() {
                    auto settingEntity = ecs.spawn_entity();

                    std::string back_path = ASSET_FILE("backgrounds/menu/setting_back.jpg");
                    ImageComponent setting_back(back_path, SETTINGS_FOCUS,
                                                [](int screenWidth, int screenHeight) {return 0;},
                                                [](int screenWidth, int screenHeight) {return 0;}, GetScreenWidth(), GetScreenHeight());
                    ecs.add_component<SettingsComponent>(settingEntity, {setting_back});
                    ecs.run_event(ChangeFocusEvent{SETTINGS_FOCUS});
                }});
        // Init background

        auto &sounds = ecs.get_components<ecs::SoundComponent>();
        for (auto &sound : sounds) {
            if (sound.has_value()) {
                sound.value().addSound("shoot", "./client/assets/sound/shoot.wav");
            }
        }

        auto &musics = ecs.get_components<ecs::MusicComponent>();
        for (auto &music : musics) {
            if (music.has_value()) {
                music.value().addMusic("game_music", "./client/assets/sound/ingame_music.wav");
                music.value().play("game_music");
                break;
            }
        }

    }

    /**
    * Close the game window
    * @param ecs
    */
    void close_game_system(Registry &ecs, const WindowCloseEvent &) {
        auto &vessels_models = ecs.get_components<VesselsComponent>();
        auto &projectiles_models = ecs.get_components<ProjectilesComponent>();
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
            if (projectiles_models[i].has_value()) {
                if (!projectiles_models[i]->drawable)
                    UnloadModel(projectiles_models[i]->model);
                TraceLog(LOG_WARNING, TextFormat("Unloaded model for entity %zu.", i));
                ecs.kill_entity(i);
            }
        }
        kill_entities_with_component<CameraComponent>(ecs);
        kill_entities_with_component<ParticleSystemComponent>(ecs);
        kill_entities_with_component<LightComponent>(ecs);
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

        auto &sounds = ecs.get_components<SoundComponent>();

        for (int i = 0; i < sounds.size();i++) {
            if (sounds[i].has_value()) {
                sounds[i].value().stop("shoot");
            }
        }

        auto &musics = ecs.get_components<MusicComponent>();

        for (int i = 0; i < musics.size();i++) {
            if (musics[i].has_value()) {
                musics[i].value().stop("game_music");
            }
        }
    }

}
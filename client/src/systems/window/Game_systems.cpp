/*
** EPITECH PROJECT, 2024
** r-type
** File description:
** Game_systems
*/

#include "ecs/Systems.hpp"
#include "game/GameState.hpp"
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
                shader = *shader_i->shader;
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
                        if (vesselComponent.drawable && !vesselComponent.is_enemy)
                        {
                            vessels_positions.emplace_back(GetWorldToScreen(vesselComponent.position, camera_i->camera));
                            vessels_health.emplace_back(vesselComponent.health);
                        }
                    }
                }
            }
        }

        BeginDrawing();

        auto &filters = ecs.get_components<FilterComponent>();
        for (auto &filter : filters) {
            if (filter.has_value()) {
                filter.value().applyFilter();
            }
        }

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
                float deltaTime = GetFrameTime();
                for (size_t i = 0; i < projectiles.size(); ++i) {
                    if (projectiles[i].has_value()) {
                        ProjectilesComponent *projectile = &projectiles[i].value();
                        if (projectile->drawable) {
                            projectile->ApplyVelocity(deltaTime);
                            if (projectile->IsAlive(camera))
                            {
                                DrawModel(projectile->model, projectile->position, 1.0f, WHITE);
                                projectile->light->UpdateLightValues(shader);
                            }
                            else
                            {
                                projectile->light->UpdateLightValues(shader, false);
                                //ecs.kill_entity(i);
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
                auto &explosions = ecs.get_components<ExplosionComponent>();
                for (int i = 0; i < explosions.size(); i++) {
                    if (explosions[i].has_value()) {
                        auto &explosion = explosions[i].value();
                        if (!explosion.active) {
                            explosion.UnloadExplosion();
                            ecs.kill_entity(i);
                        } else {
                            for (auto & model : models) {
                                if (model.has_value()) {
                                    BoundingBox hitbox = GetModelBoundingBox(model->model);

                                    explosion.update(deltaTime);
                                    explosion.draw(camera_i->camera, hitbox.max, hitbox.min);
                                }
                            }
                        }

                    }
                }
                break;
            }
        }


        DrawText(TextFormat("FPS: %d", GetFPS()), 10, 10, 20, DARKGRAY);

        auto &settings = ecs.get_components<ecs::SettingsComponent>();

        for (auto & setting : settings) {
            if (setting.has_value()) {
                setting.value().background.width = (float)GetScreenWidth();
                setting.value().background.height = (float)GetScreenHeight();
                DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.9f));
                ecs.run_event(ecs::DisplaySettingEvent{});
            }
        }

        auto &scores = ecs.get_components<ScoreComponent>();

        for (auto &score : scores) {
            if (score.has_value()) {
                score.value().draw_ingame();
            }
        }

        for (auto &filter : filters) {
            if (filter.has_value()) {
                filter.value().removeFilter();
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
                shader = *shader_i->shader;
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

        // Init background

        auto &sounds = ecs.get_components<ecs::SoundComponent>();
        for (auto &sound : sounds) {
            if (sound.has_value()) {
                sound.value().addSound("shoot", "./client/assets/sound/shoot.wav");
                sound.value().addSound("kill_enemy", "./client/assets/sound/kill_enemy_sound.wav");
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
        auto &explosions = ecs.get_components<ExplosionComponent>();

        auto gameState = getGameState(ecs);

        for (std::size_t i = 0; i < vessels_models.size(); ++i) {
            if (vessels_models[i].has_value()) {
                if (vessels_models[i]->is_enemy) {
                    if (!vessels_models[i]->drawable) {
                        UnloadModel(vessels_models[i]->model);
                        TraceLog(LOG_WARNING, TextFormat("Unloaded model for entity %zu.", i));
                    }
                } else {
                    UnloadModel(vessels_models[i]->model);
                    TraceLog(LOG_WARNING, TextFormat("Unloaded model for entity %zu.", i));
                }
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
        for (std::size_t i = 0; i < explosions.size(); i++) {
            if (explosions[i].has_value()) {
                explosions[i]->UnloadExplosion();
                ecs.kill_entity(i);
            }
        }
        kill_entities_with_component<ParticleSystemComponent>(ecs);
        kill_entities_with_component<LightComponent>(ecs);

        auto &sounds = ecs.get_components<SoundComponent>();

        for (int i = 0; i < sounds.size();i++) {
            if (sounds[i].has_value()) {
                sounds[i].value().stop("shoot");
                sounds[i].value().stop("kill_enemy");
            }
        }

        auto &musics = ecs.get_components<MusicComponent>();

        for (int i = 0; i < musics.size();i++) {
            if (musics[i].has_value()) {
                musics[i].value().stop("game_music");
            }
        }

        auto &scores = ecs.get_components<ScoreComponent>();

        for (auto &score : scores) {
            if (score.has_value()) {
                score->score = 0;
            }
        }

        auto user = gameState->get().getUser();
        user.total_score += user.score;
        gameState->get().updateUser(user);

        auto players = gameState->get().getOtherPlayer();
        for (auto &player: players) {
            player.total_score + player.score;
        }
        gameState->get().updateOtherPlayer(players);
    }

}
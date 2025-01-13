/*
** EPITECH PROJECT, 2024
** r-type
** File description:
** Lobby_systems
*/

#include "ecs/Systems.hpp"
#include "game/GameState.hpp"

namespace ecs {

    /**
     * Draw the lobby window
     * @param ecs
     */
    void draw_lobby_system(Registry &ecs, const WindowDrawEvent &) {
        ecs.run_event(ControlsEvent{});
        auto &backgrounds = ecs.get_components<BackgroundComponent>();
        auto &decors = ecs.get_components<DecorElementComponent>();
        auto &musics = ecs.get_components<MusicComponent>();

        for (auto &music : musics) {
            if (music.has_value()) {
                music.value().update("menu_music");
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

        auto gameState = getGameState(ecs);

        if (gameState->get().getGameState() == network::LobbyGameState::Starting) {
            int ringWidth = 400;
            int ringHeight = 400;
            int fontSize = 24;
            static float startAngle = 0;
            static float endAngle = 270;
            startAngle += 2;
            endAngle += 2;
            if (startAngle >= 360) {
                startAngle = 0;
                endAngle = 270;
            }
            Color blackTransparent = {0, 48, 73, 200};
            DrawRectangle(0, 0, screenWidth, screenHeight, blackTransparent);
            Vector2 center = {screenWidth / 2.0f, screenHeight / 2.0f};
            DrawText("Starting...", center.x - MeasureText("Starting...", fontSize) / 2, center.y - fontSize / 2, fontSize, BLACK);
            DrawRing(center, 80, 190, startAngle, endAngle, 0, Fade(RED, 0.5f));
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
        auto &cameras = ecs.get_components<CameraComponent>();
        Camera camera = {};
        for (auto & camera_i : cameras) {
            if (camera_i.has_value()) {
                camera_i->camera.position = {10.0f, 10.0f, 30.0f};
                camera_i->camera.target = {-10.0f, 0.0f, 0.0f};
                camera_i->camera.up = {0.0f, 30.0f, 0.0f};
                camera_i->camera.fovy = 45.0f;
                camera_i->camera.projection = CAMERA_PERSPECTIVE;
                camera = camera_i->camera;
                break;
            }
        }

        ecs.run_event(InitLightEvent{client::LIGHT_DIRECTIONAL, {0, 0, 0},
        Vector3Normalize(Vector3Subtract(camera.target, camera.position)), WHITE, 0});

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
    }

    /**
     * Close the lobby window
     * @param ecs
     */
    void close_lobby_system(Registry &ecs, const WindowCloseEvent &) {
        kill_entities_with_component<LightComponent>(ecs);
        kill_entities_with_component<TextComponent>(ecs);
        kill_entities_with_component<ButtonComponent>(ecs);

        auto &musics = ecs.get_components<MusicComponent>();

        for (int i = 0; i < musics.size();i++) {
            if (musics[i].has_value()) {
                musics[i].value().stop("menu_music");
                ecs.kill_entity(i);
            }
        }
    }
}

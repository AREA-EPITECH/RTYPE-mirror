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
        auto &lights = ecs.get_components<LightComponent>();
        auto &shaders = ecs.get_components<ShaderComponent>();

        Shader shader = {};

        for (auto &shader_i : shaders) {
            if (shader_i.has_value()) {
                shader = *shader_i->shader;
                break;
            }
        }

        for (auto &light : lights) {
            if (light.has_value()) {
                light->light->UpdateLightValues(shader, light->light->_enabled);
            }
        }

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
                int j = 0;
                for (auto &model : models) {
                    if (model.has_value()) {
                        VesselsComponent &modelComponent = model.value();
                        if (modelComponent.drawable) {
                            Matrix rotation = MatrixRotate((Vector3){0.0f, 1.0f, 0.0f}, rotationAngle);
                            modelComponent.model.transform = MatrixMultiply(modelComponent.model.transform, rotation);
                            DrawModel(modelComponent.model, {0, 0, 0}, 1.0f, WHITE);
                        }
                    }
                    j++;
                }

                EndMode3D();
                break;
            }
        }

        auto gameState = getGameState(ecs);

        if (gameState->get().getGameState() == game::GameState::LobbyGameState::Starting) {
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
            DrawText("Starting...", center.x - MeasureText("Starting...", fontSize) / 2, center.y - fontSize / 2, fontSize, WHITE);
            DrawRing(center, 80, 190, startAngle, endAngle, 0, Fade(RED, 0.5f));
        }


        if (gameState->get().getShowScore()) {
            int scoreBoardWidth = screenWidth / 1.5;
            int scoreBoardHeight = screenHeight / 1.5;
            int headerHeight = scoreBoardHeight / 10;
            int fontSize = 52;
            Color blackTransparent = {0, 48, 73, 200};
            DrawRectangle(0, 0, screenWidth, screenHeight, blackTransparent);
            Vector2 center = {screenWidth / 2.0f, screenHeight / 2.0f};
            DrawRectangle(center.x - scoreBoardWidth / 2, center.y - scoreBoardHeight / 2, scoreBoardWidth, scoreBoardHeight, BLACK);
            DrawRectangleLines(center.x - scoreBoardWidth / 2, center.y - scoreBoardHeight / 2, scoreBoardWidth, scoreBoardHeight, WHITE);
            DrawRectangleLines(center.x - scoreBoardWidth / 2, center.y - scoreBoardHeight / 2, scoreBoardWidth, headerHeight, WHITE);
            auto &scores = ecs.get_components<ecs::ScoreComponent>();
            int level = 0;
            for (auto &score_i: scores) {
                if (score_i.has_value()) {
                    level = score_i->level;
                    break;
                }
            }
            std::string level_str = fmt::format("Level {}", level);
            DrawText("LEADERBOARD", center.x - MeasureText("LEADERBOARD", fontSize) / 2, center.y - scoreBoardHeight / 2 + headerHeight / 2 - fontSize / 2, fontSize, WHITE);
            DrawText(level_str.c_str(), center.x - scoreBoardWidth / 2 + 20, center.y - scoreBoardHeight / 2 + headerHeight / 2 - fontSize / 2, fontSize, WHITE);
            auto &buttons = ecs.get_components<ecs::CloseLeaderBoard>();
            for (int i = 0; i < buttons.size(); i++) {
                if (buttons[i].has_value()) {
                    auto &button = buttons[i].value();
                    button.drawButton(ecs::get_focus(ecs));
                }
            }
            std::vector<std::tuple<std::string, int, bool>> lines;
            int startY = center.y - scoreBoardHeight / 2 + headerHeight + 10;
            fontSize = 64;
            auto user = gameState->get().getUser();
            lines.push_back({user.name, user.score, true});
            auto other_players = gameState->get().getOtherPlayer();
            for (auto &player: other_players) {
                lines.push_back({player.name, player.score, false});
            }
            std::sort(lines.begin(), lines.end(), [](std::tuple<std::string, int, bool> a, std::tuple<std::string, int, bool> b){
                return std::get<1>(a) > std::get<1>(b);
            });
            for (auto &line: lines) {
                DrawText(std::get<0>(line).c_str(), center.x - scoreBoardWidth / 2 + 20, startY, fontSize, std::get<2>(line) ? GREEN : WHITE);
                DrawText(std::to_string(std::get<1>(line)).c_str(), center.x + scoreBoardWidth / 2 - 20 - MeasureText(std::to_string(std::get<1>(line)).c_str(), fontSize), startY, fontSize, WHITE);
                startY += fontSize + 10;
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
     * Open the lobby window
     * @param ecs
     */
    void open_lobby_system(Registry &ecs, const WindowOpenEvent &) {
        auto &cameras = ecs.get_components<CameraComponent>();
        Camera camera = {};
        for (auto &camera_i : cameras)
        {
            if (camera_i.has_value())
            {
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

        auto &backgrounds = ecs.get_components<BackgroundComponent>();
        for (auto & background : backgrounds) {
            if (background.has_value()) {
                background->speed = 50;
            }
        }

        auto &decors = ecs.get_components<DecorElementComponent>();
        for (auto & decor : decors) {
            if (decor.has_value()) {
                switch (decor->depth)
                {
                    case 1:
                        decor->speed = 75;
                        break;
                    case 2:
                        decor->speed = 100;
                        break;
                    case 3:
                        decor->speed = 125;
                        break;
                    default:
                        break;
                }
            }
        }

        auto &musics = ecs.get_components<MusicComponent>();
        for (int i = 0; i < musics.size();i++) {
            if (musics[i].has_value()) {
                if (!musics[i].value().isPlaying("menu_music")) {
                    musics[i].value().play("menu_music");
                }
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
                musics[i].value().pause("menu_music");
            }
        }
    }
}

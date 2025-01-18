/*
** EPITECH PROJECT, 2024
** r-type
** File description:
** End_game_systems
*/

#include "ecs/Systems.hpp"
#include "game/GameState.hpp"

namespace ecs {
    void draw_endgame_system(Registry &ecs, const WindowDrawEvent &) {
        auto &musics = ecs.get_components<ecs::MusicComponent>();
        auto &backgrounds = ecs.get_components<BackgroundComponent>();
        auto &decors = ecs.get_components<DecorElementComponent>();
        for (auto &music : musics) {
            if (music.has_value()) {
                music.value().update("win_music");
            }
        }

        for (auto &background : backgrounds)
        {
            if (background.has_value())
            {
                background->offset -= background->speed * GetFrameTime();
                float textureWidthOnScreen = static_cast<float>(background->texture.width) *
                    (static_cast<float>(GetScreenHeight()) / static_cast<float>(background->texture.height));
                if (background->offset <= -textureWidthOnScreen)
                {
                    background->offset = 0;
                }
            }
        }
        float deltaTime = GetFrameTime();
        for (auto &decor : decors)
        {
            if (decor.has_value())
            {
                decor->Update(deltaTime, GetScreenWidth(), GetScreenHeight());
            }
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        const int screenWidth = GetScreenWidth();
        const int screenHeight = GetScreenHeight();

        auto gameState = getGameState(ecs);

        for (auto &background : backgrounds)
        {
            if (background.has_value())
            {
                background->DrawLayer(screenWidth, screenHeight);
            }
        }
        for (auto &decor : decors)
        {
            if (decor.has_value())
            {
                decor->DrawDecorElement(screenWidth, screenHeight);
            }
        }

        int scoreBoardWidth = screenWidth / 1.5;
        int scoreBoardHeight = screenHeight / 1.5;
        int headerHeight = scoreBoardHeight / 10;
        int fontSize = 52;
        Vector2 center = {screenWidth / 2.0f, screenHeight / 2.0f};
        DrawText("GAME OVER", center.x - MeasureText("GAME OVER", fontSize + 64) / 2, (center.y - scoreBoardHeight / 2) / 2 - (fontSize + 64) / 2, fontSize + 64, WHITE);
        DrawRectangle(center.x - scoreBoardWidth / 2, center.y - scoreBoardHeight / 2, scoreBoardWidth, scoreBoardHeight, Fade(BLACK, 0.5f));
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
        DrawText("LEADERBOARD", center.x - MeasureText("LEADERBOARD", fontSize) / 2, center.y - scoreBoardHeight / 2 + headerHeight / 2 - fontSize / 2, fontSize, WHITE);
        std::vector<std::tuple<std::string, int, bool>> lines;
        int startY = center.y - scoreBoardHeight / 2 + headerHeight + 10;
        fontSize = 64;
        auto user = gameState->get().getUser();
        lines.push_back({user.name, user.total_score, true});
        auto other_players = gameState->get().getOtherPlayer();
        for (auto &player: other_players) {
            lines.push_back({player.name, player.total_score, false});
        }
        std::sort(lines.begin(), lines.end(), [](std::tuple<std::string, int, bool> a, std::tuple<std::string, int, bool> b){
            return std::get<1>(a) > std::get<1>(b);
        });
        for (auto &line: lines) {
            DrawText(std::get<0>(line).c_str(), center.x - scoreBoardWidth / 2 + 20, startY, fontSize, std::get<2>(line) ? GREEN : WHITE);
            DrawText(std::to_string(std::get<1>(line)).c_str(), center.x + scoreBoardWidth / 2 - 20 - MeasureText(std::to_string(std::get<1>(line)).c_str(), fontSize), startY, fontSize, WHITE);
            startY += fontSize + 10;
        }

        auto &buttons = ecs.get_components<ButtonComponent>();
        for (auto &button: buttons) {
            if (button.has_value()) {
                button->drawButton(ecs::get_focus(ecs));
            }
        }
        EndDrawing();
    }

    void open_endgame_system(Registry &ecs, const WindowOpenEvent &) {
        kill_entities_with_component<ecs::ShowBoxComponent>(ecs);
        kill_entities_with_component<ecs::TextInputComponent>(ecs);
        kill_entities_with_component<ecs::ButtonComponent>(ecs);
        kill_entities_with_component<ecs::TextComponent>(ecs);
        kill_entities_with_component<ecs::CloseLeaderBoard>(ecs);
        kill_entities_with_component<ecs::VesselsComponent>(ecs);
        kill_entities_with_component<ecs::ProjectilesComponent>(ecs);
        kill_entities_with_component<ecs::ControllableComponent>(ecs);
        kill_entities_with_component<ecs::MenuText>(ecs);
        kill_entities_with_component<ecs::ShaderComponent>(ecs);
        kill_entities_with_component<ecs::CameraComponent>(ecs);
        kill_entities_with_component<ecs::LightComponent>(ecs);
        kill_entities_with_component<ecs::BackgroundComponent>(ecs);
        kill_entities_with_component<ecs::DecorElementComponent>(ecs);
        kill_entities_with_component<ecs::HealthBarComponent>(ecs);
        kill_entities_with_component<ecs::ParticleSystemComponent>(ecs);
        kill_entities_with_component<ecs::ImageComponent>(ecs);
        kill_entities_with_component<ecs::FilterComponent>(ecs);

        // Init background
        ecs.run_event(InitBackgroundEvent{"client/assets/backgrounds/game/space_background.png", 1, 50, 0});
        ecs.run_event(InitDecorElementEvent{"client/assets/backgrounds/game/space_midground.png", 1, 75});
        ecs.run_event(InitDecorElementEvent{"client/assets/backgrounds/game/space_midground_2.png", 2, 100});
        ecs.run_event(InitDecorElementEvent{"client/assets/backgrounds/game/space_foreground.png", 3, 125});

        auto &musics = ecs.get_components<ecs::MusicComponent>();
        for (auto &music : musics) {
            if (music.has_value()) {
                music.value().addMusic("win_music", "./client/assets/sound/win_music.wav");
                music.value().play("win_music");
                break;
            }
        }

        auto returnButton = ecs.spawn_entity();

        int buttonWidth = 600;
        int buttonHeight = 100;

        ecs.add_component<ecs::ButtonComponent>(
        returnButton,
        ecs::ButtonComponent(
            buttonWidth, buttonHeight, "RETURN TO MENU", ecs::END_GAME_FOCUS, [&ecs]()
            {
                change_window(ecs, ecs::WindowType::MENU);
            }, [buttonWidth](int screenWidth, int screenHeight)
            {
                Vector2 center = {screenWidth / 2.0f, screenHeight / 2.0f};
                return center.x - (buttonWidth / 2);
            }, [buttonHeight](int screenWidth, int screenHeight)
            {
                Vector2 center = {screenWidth / 2.0f, screenHeight / 2.0f};
                int scoreBoardHeight = screenHeight / 1.5;
                return screenHeight - (screenHeight - (center.y + scoreBoardHeight / 2)) / 2 - buttonHeight / 2;
            }, 48, 4, ecs::ButtonStyleColors.at(ecs::BUTTON_BASE_COLOR_NORMAL),
            ecs::ButtonStyleColors.at(ecs::BUTTON_TEXT_COLOR_NORMAL),
            ecs::ButtonStyleColors.at(ecs::BUTTON_BORDER_COLOR_NORMAL),
            ecs::ButtonStyleColors.at(ecs::BUTTON_BASE_COLOR_FOCUSED),
            ecs::ButtonStyleColors.at(ecs::BUTTON_TEXT_COLOR_FOCUSED),
            ecs::ButtonStyleColors.at(ecs::BUTTON_BORDER_COLOR_FOCUSED)));
    }

    void close_endgame_system(Registry &ecs, const WindowCloseEvent &) {
        auto &musics = ecs.get_components<ecs::MusicComponent>();
        for (auto &music : musics) {
            if (music.has_value()) {
                music.value().stop("win_music");
                break;
            }
        }
        kill_entities_with_component<ecs::ButtonComponent>(ecs);
        kill_entities_with_component<ecs::BackgroundComponent>(ecs);
        kill_entities_with_component<ecs::DecorElementComponent>(ecs);

        // Reset gamestate;
        auto gameState = getGameState(ecs);
        auto user = gameState->get().getUser();
        game::GameState::Player cpy_user;
        cpy_user.id = user.id;
        kill_entities_with_component<game::GameState>(ecs);
        auto gamestateEntity = ecs.spawn_entity();
        ecs.add_component<game::GameState>(gamestateEntity, {});
        gameState = getGameState(ecs);
        gameState->get().updateUser(cpy_user);

        // Reset Score component
        auto &scores = ecs.get_components<ecs::ScoreComponent>();
        for (auto &score : scores) {
            if (score.has_value()) {
                score->level = 1;
                score->score = 0;
                score->win_score = 0;
                break;
            }
        }
    }
}
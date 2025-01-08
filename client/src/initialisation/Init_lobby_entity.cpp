/*
** EPITECH PROJECT, 2024
** r-type
** File description:
** init_lobby_entity
*/

#include "ecs/Systems.hpp"
#include "game/GameState.hpp"

void init_lobby_entity(Registry &ecs)
{
    auto &gameStateCps = ecs.get_components<game::GameState>();
    std::optional<std::reference_wrapper<game::GameState>> gameState;
    for (auto &it : gameStateCps) {
        if (it.has_value()) {
            gameState = std::ref(*it);
            break;
        }
    }
    auto idText = ecs.spawn_entity();
    std::string idStr = "ID : ";
    ecs.add_component<ecs::TextComponent>(idText, {idStr, 54, 50, 50, 2});

    std::vector<std::string> players_name;
    players_name.push_back(gameState->get().getUser().name);
    for (const auto &i : players_name)
    {
        auto playerId = ecs.spawn_entity();

        ecs.add_component<ecs::TextComponent>(playerId, {i, 54, 50, 50, 1});
    }

    auto readyButton = ecs.spawn_entity();

    int buttonWidth = 500;
    int buttonHeight = 100;


    ecs::TextComponent readyText("Ready", 54, 0, 0);
    ecs.add_component<ecs::ButtonComponent>(
        readyButton,
        ecs::ButtonComponent(
            buttonWidth, buttonHeight, "Ready", ecs::LOBBY_FOCUS, [&ecs]()
            { ecs::change_window(ecs, ecs::WindowType::GAME); }, [buttonWidth](int screenWidth, int screenHeight)
            { return screenWidth * 0.66 - (buttonWidth / 2) + 20; }, [](int screenWidth, int screenHeight)
            { return screenHeight - 150; }, 24, 4, ecs::ButtonStyleColors.at(ecs::BUTTON_BASE_COLOR_NORMAL),
            ecs::ButtonStyleColors.at(ecs::BUTTON_TEXT_COLOR_NORMAL),
            ecs::ButtonStyleColors.at(ecs::BUTTON_BORDER_COLOR_NORMAL),
            ecs::ButtonStyleColors.at(ecs::BUTTON_BASE_COLOR_FOCUSED),
            ecs::ButtonStyleColors.at(ecs::BUTTON_TEXT_COLOR_FOCUSED),
            ecs::ButtonStyleColors.at(ecs::BUTTON_BORDER_COLOR_FOCUSED)));
    std::cout << "here" << std::endl;
}

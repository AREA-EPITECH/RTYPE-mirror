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
    auto gameState = getGameState(ecs);
    auto idText = ecs.spawn_entity();
    std::string idStr = "ID : ";
    ecs.add_component<ecs::TextComponent>(idText, {idStr, 54, 50, 50, 2});

    std::vector<std::string> players_name;
    players_name.push_back(gameState->get().getUser().name);
    players_name.push_back("");
    players_name.push_back("");
    players_name.push_back("");
    for (const auto &i : players_name)
    {
        auto playerId = ecs.spawn_entity();

        ecs.add_component<ecs::TextComponent>(playerId, {i, 54, 50, 50, 1});
    }

    auto readyButton = ecs.spawn_entity();

    int buttonWidth = 500;
    int buttonHeight = 100;

    ecs.add_component<ecs::ButtonComponent>(
        readyButton,
        ecs::ButtonComponent(
            buttonWidth, buttonHeight, "Ready", ecs::LOBBY_FOCUS, [&ecs, readyButton]()
            {
                auto &buttons = ecs.get_components<ecs::ButtonComponent>();
                struct network::LobbyActionPacket packet;
                auto gameState = getGameState(ecs);
                auto user = gameState->get().getUser();
                packet.actionType = network::LobbyActionType::ChangeReady;
                if (user.is_ready) {
                    packet.readyState = false;
                    user.is_ready = false;
                    buttons[readyButton].value().setText("Ready");
                } else {
                    packet.readyState = true;
                    user.is_ready = true;
                    buttons[readyButton]->setText("Not Ready");
                }
                packet.roomId = gameState->get().getRoomId();
                ecs.run_event(packet);
                gameState->get().updateUser(user);
                //ecs::change_window(ecs, ecs::WindowType::GAME);
            }, [buttonWidth](int screenWidth, int screenHeight)
            { return screenWidth * 0.66 - (buttonWidth / 2) + 20; }, [](int screenWidth, int screenHeight)
            { return screenHeight - 150; }, 24, 4, ecs::ButtonStyleColors.at(ecs::BUTTON_BASE_COLOR_NORMAL),
            ecs::ButtonStyleColors.at(ecs::BUTTON_TEXT_COLOR_NORMAL),
            ecs::ButtonStyleColors.at(ecs::BUTTON_BORDER_COLOR_NORMAL),
            ecs::ButtonStyleColors.at(ecs::BUTTON_BASE_COLOR_FOCUSED),
            ecs::ButtonStyleColors.at(ecs::BUTTON_TEXT_COLOR_FOCUSED),
            ecs::ButtonStyleColors.at(ecs::BUTTON_BORDER_COLOR_FOCUSED)));
    auto nextShipButton = ecs.spawn_entity();
    ecs.add_component<ecs::ButtonComponent>(
        nextShipButton,
        ecs::ButtonComponent(
            100, 400, ">", ecs::LOBBY_FOCUS, [&ecs]()
            { next_ship(ecs); }, [buttonWidth](int screenWidth, int screenHeight)
            { return screenWidth - 120; }, [](int screenWidth, int screenHeight)
            { return (screenHeight - 400) / 2; }, 24, 4, ecs::ButtonStyleColors.at(ecs::BUTTON_BASE_COLOR_NORMAL),
            ecs::ButtonStyleColors.at(ecs::BUTTON_TEXT_COLOR_NORMAL),
            ecs::ButtonStyleColors.at(ecs::BUTTON_BORDER_COLOR_NORMAL),
            ecs::ButtonStyleColors.at(ecs::BUTTON_BASE_COLOR_FOCUSED),
            ecs::ButtonStyleColors.at(ecs::BUTTON_TEXT_COLOR_FOCUSED),
            ecs::ButtonStyleColors.at(ecs::BUTTON_BORDER_COLOR_FOCUSED)));
    auto prevShipButton = ecs.spawn_entity();
    ecs.add_component<ecs::ButtonComponent>(
        prevShipButton,
        ecs::ButtonComponent(
            100, 400, "<", ecs::LOBBY_FOCUS, [&ecs]()
            { previous_ship(ecs); }, [buttonWidth](int screenWidth, int screenHeight)
            { return screenWidth / 3 + 20; }, [](int screenWidth, int screenHeight)
            { return (screenHeight - 400) / 2; }, 24, 4, ecs::ButtonStyleColors.at(ecs::BUTTON_BASE_COLOR_NORMAL),
            ecs::ButtonStyleColors.at(ecs::BUTTON_TEXT_COLOR_NORMAL),
            ecs::ButtonStyleColors.at(ecs::BUTTON_BORDER_COLOR_NORMAL),
            ecs::ButtonStyleColors.at(ecs::BUTTON_BASE_COLOR_FOCUSED),
            ecs::ButtonStyleColors.at(ecs::BUTTON_TEXT_COLOR_FOCUSED),
            ecs::ButtonStyleColors.at(ecs::BUTTON_BORDER_COLOR_FOCUSED)));
}

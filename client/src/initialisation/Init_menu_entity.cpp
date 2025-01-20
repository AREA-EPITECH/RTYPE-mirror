/*
** EPITECH PROJECT, 2024
** r-type
** File description:
** init_menu_entity
*/

#include "raylib/raygui.h"
#include "ecs/Systems.hpp"
#include "game/GameState.hpp"

void init_menu_entity(Registry &ecs)
{
    auto JoinRoom = ecs.spawn_entity();

    int buttonWidth = 500;
    int buttonHeight = 100;

    std::string message = "Enter room ID:";
    auto boxColor = DARKGRAY;
    auto textColor = WHITE;

    auto showBoxEntity = ecs.spawn_entity();
    Rectangle boxRect = {100, 100, 600, 200};
    ecs.add_component<ecs::ShowBoxComponent>(
        showBoxEntity,
        ecs::ShowBoxComponent(
            boxRect, message, boxColor, textColor, ecs::JOIN_ROOM_FOCUS, "Id...", "Close", "Continue",
            [boxRect](int screenWidth, int screenHeight) { return (float)screenWidth / 2 - (boxRect.width / 2); },
            [boxRect](int screenWidth, int screenHeight) { return (float)screenHeight / 2 - (boxRect.height / 2); },
            [&ecs, showBoxEntity]() {
                struct network::LobbyActionPacket packet;
                auto &input = ecs.get_components<ecs::TextInputComponent>();
                for (auto &it : input)
                {
                    if (it.has_value())
                    {
                        packet.name = it->text;
                    }
                }
                if (packet.name.empty())
                {
                    return;
                }
                auto &boxCpt = ecs.get_components<ecs::ShowBoxComponent>();
                packet.roomId = boxCpt[showBoxEntity]->numberInput.value;
                auto &gameStateCps = ecs.get_components<game::GameState>();
                auto gameState = getGameState(ecs);
                if (gameState) {
                    game::GameState::Player user = gameState->get().getUser();
                    user.is_ready = false;
                    user.name = packet.name;
                    user.ship_id = 0;
                    gameState->get().updateUser(user);
                    gameState->get().setRoomId(packet.roomId);
                }
                packet.actionType = network::LobbyActionType::JoinRoom;
                ecs.run_event(packet);
                change_window(ecs, ecs::WindowType::LOBBY);
            },
            [&ecs]() {ecs.run_event(ecs::ChangeFocusEvent{ecs::MENU_FOCUS}); }));

    auto textInput = ecs.spawn_entity();
    ecs.add_component<ecs::TextInputComponent>(textInput,
                                               ecs::TextInputComponent(
                                                   {0, 0, 500, 100}, ecs::MENU_FOCUS, "Enter your name...", 20,
                                                   [buttonWidth](int screenWidth, int screenHeight)
                                                   { return screenWidth / 2 - (buttonWidth / 2); },
                                                   [](int screenWidth, int screenHeight) { return screenHeight / 3; },
                                                   24, 4, 20, ecs::InputStyleColors.at(ecs::INPUT_BASE_COLOR_NORMAL),
                                                   ecs::InputStyleColors.at(ecs::INPUT_TEXT_COLOR_NORMAL),
                                                   ecs::InputStyleColors.at(ecs::INPUT_BORDER_COLOR_NORMAL),
                                                   ecs::InputStyleColors.at(ecs::INPUT_BASE_COLOR_FOCUSED),
                                                   ecs::InputStyleColors.at(ecs::INPUT_TEXT_COLOR_FOCUSED),
                                                   ecs::InputStyleColors.at(ecs::INPUT_BORDER_COLOR_FOCUSED),
                                                   ecs::InputStyleColors.at(ecs::INPUT_BASE_COLOR_PRESSED),
                                                   ecs::InputStyleColors.at(ecs::INPUT_TEXT_COLOR_PRESSED),
                                                   ecs::InputStyleColors.at(ecs::INPUT_BORDER_COLOR_PRESSED)));

    ecs::TextComponent joinText("Join room", 54, 0, 0);
    ecs.add_component<ecs::ButtonComponent>(
        JoinRoom,
        ecs::ButtonComponent(
            buttonWidth, buttonHeight, "Join room", ecs::MENU_FOCUS,
            [&ecs]()
            {
                ecs.run_event(ecs::ChangeFocusEvent{ecs::JOIN_ROOM_FOCUS});
                auto &showbox = ecs.get_components<ecs::ShowBoxComponent>();
                for (auto &i : showbox)
                {
                    if (i.has_value())
                    {
                        i.value().isVisible = true;
                    }
                }
            },
            [buttonWidth](int screenWidth, int screenHeight) { return screenWidth / 2 - (buttonWidth / 2); },
            [buttonHeight](int screenWidth, int screenHeight) { return screenHeight / 3 + (buttonHeight + 50); }, 24, 4,
            ecs::ButtonStyleColors.at(ecs::BUTTON_BASE_COLOR_NORMAL),
            ecs::ButtonStyleColors.at(ecs::BUTTON_TEXT_COLOR_NORMAL),
            ecs::ButtonStyleColors.at(ecs::BUTTON_BORDER_COLOR_NORMAL),
            ecs::ButtonStyleColors.at(ecs::BUTTON_BASE_COLOR_FOCUSED),
            ecs::ButtonStyleColors.at(ecs::BUTTON_TEXT_COLOR_FOCUSED),
            ecs::ButtonStyleColors.at(ecs::BUTTON_BORDER_COLOR_FOCUSED)));

    auto CreateRoom = ecs.spawn_entity();

    ecs::TextComponent createText("Create room", 54, 0, 0);
    ecs.add_component<ecs::ButtonComponent>(
        CreateRoom,
        ecs::ButtonComponent(
            buttonWidth, buttonHeight, "Create room", ecs::MENU_FOCUS,
            [&ecs]()
            {
                struct network::LobbyActionPacket packet;
                auto &input = ecs.get_components<ecs::TextInputComponent>();
                for (auto &it : input)
                {
                    if (it.has_value())
                    {
                        packet.name = it->text;
                    }
                }
                if (packet.name.empty())
                {
                    return;
                }
                auto gameState = getGameState(ecs);
                if (gameState) {
                    game::GameState::Player user = gameState->get().getUser();
                    user.is_ready = false;
                    user.name = packet.name;
                    user.ship_id = 0;
                    gameState->get().updateUser(user);
                }
                packet.actionType = network::LobbyActionType::CreateRoom;
                ecs.run_event(packet);
                change_window(ecs, ecs::WindowType::LOBBY);
            },
            [buttonWidth](int screenWidth, int screenHeight) { return screenWidth / 2 - (buttonWidth / 2); },
            [buttonHeight](int screenWidth, int screenHeight) { return screenHeight / 3 + (buttonHeight * 2 + 100); },
            24, 4, ecs::ButtonStyleColors.at(ecs::BUTTON_BASE_COLOR_NORMAL),
            ecs::ButtonStyleColors.at(ecs::BUTTON_TEXT_COLOR_NORMAL),
            ecs::ButtonStyleColors.at(ecs::BUTTON_BORDER_COLOR_NORMAL),
            ecs::ButtonStyleColors.at(ecs::BUTTON_BASE_COLOR_FOCUSED),
            ecs::ButtonStyleColors.at(ecs::BUTTON_TEXT_COLOR_FOCUSED),
            ecs::ButtonStyleColors.at(ecs::BUTTON_BORDER_COLOR_FOCUSED)));
}

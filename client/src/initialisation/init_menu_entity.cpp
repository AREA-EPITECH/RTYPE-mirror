/*
** EPITECH PROJECT, 2024
** r-type
** File description:
** init_menu_entity
*/

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

    auto textInput = ecs.spawn_entity();
    ecs.add_component<ecs::TextInputComponent>(
        textInput,
        ecs::TextInputComponent(
            {0, 0, 500, 100}, ecs::MENU_FOCUS, "Enter your name...", 20, Color{120, 0, 0, 255}, Color{253, 240, 213, 255}, BLACK,
            [buttonWidth](int screenWidth, int screenHeight) { return screenWidth / 2 - (buttonWidth / 2); },
            [](int screenWidth, int screenHeight) { return screenHeight / 3; }));

    auto showBoxEntity = ecs.spawn_entity();
    Rectangle boxRect = {100, 100, 600, 200};
    ecs.add_component<ecs::ShowBoxComponent>(
        showBoxEntity,
        ecs::ShowBoxComponent(
            boxRect, message, boxColor, textColor, ecs::JOIN_ROOM_FOCUS, "Id...", "Close", "Continue",
            nullptr,
            [&ecs, showBoxEntity, textInput]() {
                struct network::LobbyActionPacket packet;
                auto inputs = ecs.get_components<ecs::ShowBoxComponent>();
                auto component = inputs[showBoxEntity];
                if (component.has_value())
                {
                    packet.roomId = (uint32_t)component->numberInput.value;
                }
                auto &input = ecs.get_components<ecs::TextInputComponent>();
                if (input[textInput].has_value())
                {
                    packet.name = input[textInput]->text;
                }
                if (!packet.name.length() || !packet.roomId)
                {
                    return;
                }
                auto &gameStateCps = ecs.get_components<game::GameState>();
                std::optional<std::reference_wrapper<game::GameState>> gameState;
                for (auto &it : gameStateCps) {
                    if (it.has_value()) {
                        gameState = std::ref(*it);
                        break;
                    }
                }
                if (gameState) {
                    game::GameState::Player player = gameState->get().getUser();
                    player.name = packet.name;
                    gameState->get().updateUser(player);
                }
                packet.actionType = network::LobbyActionType::JoinRoom;
                ecs.run_event(packet);
                ecs::change_window(ecs, ecs::WindowType::LOBBY);
            },
            [boxRect](int screenWidth, int screenHeight) { return (float)screenWidth / 2 - (boxRect.width / 2); },
            [boxRect](int screenWidth, int screenHeight) { return (float)screenHeight / 2 - (boxRect.height / 2); }));

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
            [buttonHeight](int screenWidth, int screenHeight) { return screenHeight / 3 + (buttonHeight + 50); }));

    auto CreateRoom = ecs.spawn_entity();

    ecs::TextComponent createText("Create room", 54, 0, 0);
    ecs.add_component<ecs::ButtonComponent>(
        CreateRoom,
        ecs::ButtonComponent(
            buttonWidth, buttonHeight, "Create room", ecs::MENU_FOCUS, [&ecs, textInput]() {
                struct network::LobbyActionPacket packet;
                auto &input = ecs.get_components<ecs::TextInputComponent>();
                if (input[textInput].has_value())
                    packet.name = input[textInput]->text;
                if (!packet.name.length())
                    return;
                auto &gameStateCps = ecs.get_components<game::GameState>();
                std::optional<std::reference_wrapper<game::GameState>> gameState;
                for (auto &it : gameStateCps) {
                    if (it.has_value()) {
                        gameState = std::ref(*it);
                        break;
                    }
                }
                if (gameState) {
                    game::GameState::Player player = gameState->get().getUser();
                    player.name = packet.name;
                    gameState->get().updateUser(player);
                }
                packet.actionType = network::LobbyActionType::CreateRoom;
                ecs.run_event(packet);
                change_window(ecs, ecs::WindowType::LOBBY);
            },
            [buttonWidth](int screenWidth, int screenHeight) { return screenWidth / 2 - (buttonWidth / 2); },
            [buttonHeight](int screenWidth, int screenHeight) { return screenHeight / 3 + (buttonHeight * 2 + 100); }));
}

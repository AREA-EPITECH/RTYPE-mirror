/*
** EPITECH PROJECT, 2024
** r-type
** File description:
** init_menu_entity
*/

#include "ecs/Systems.hpp"

void init_menu_entity(Registry &ecs)
{
    auto JoinRoom = ecs.spawn_entity();

    int buttonWidth = 500;
    int buttonHeight = 100;

    std::string message = "Enter room ID:";
    auto boxColor = DARKGRAY;
    auto textColor = WHITE;

    ecs::TextInputComponent textBoxInput({500, 100, 500, 50},
                                         "Enter room id",
                                         20,
                                         Color{120, 0, 0, 255},
                                         Color{253, 240, 213, 255},
                                         BLACK);

    ecs::ButtonComponent closeButton(buttonWidth, buttonHeight, "Close", []() {});

    ecs::ButtonComponent continueButton(buttonWidth, buttonHeight, "Continue", []() {});

    auto showBoxEntity = ecs.spawn_entity();
    Rectangle boxRect = {100, 100, 600, 300};
    ecs.add_component<ecs::ShowBoxComponent>(
        showBoxEntity,
        ecs::ShowBoxComponent(
            boxRect, message, boxColor, textColor, "", "Close", "Continue",
            [boxRect](int screenWidth, int screenHeight) { return (float)screenWidth / 2 - (boxRect.width / 2); },
            [boxRect](int screenWidth, int screenHeight) { return (float)screenHeight / 2 - (boxRect.height / 2); }));

    auto textInput = ecs.spawn_entity();
    ecs.add_component<ecs::TextInputComponent>(
        textInput,
        ecs::TextInputComponent(
            {0, 0, 500, 100}, "Enter your name...", 20, Color{120, 0, 0, 255}, Color{253, 240, 213, 255}, BLACK,
            [buttonWidth](int screenWidth, int screenHeight) { return screenWidth / 2 - (buttonWidth / 2); },
            [](int screenWidth, int screenHeight) { return screenHeight / 3; }));

    ecs::TextComponent joinText("Join room", 54, 0, 0);
    ecs.add_component<ecs::ButtonComponent>(
        JoinRoom,
        ecs::ButtonComponent(
            buttonWidth, buttonHeight, "Join room",
            [&ecs]()
            {
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

    std::cout << textInput << std::endl;
    ecs::TextComponent createText("Create room", 54, 0, 0);
    ecs.add_component<ecs::ButtonComponent>(
        CreateRoom,
        ecs::ButtonComponent(
            buttonWidth, buttonHeight, "Create room", [&ecs]() {
                struct network::LobbyActionPacket packet;
                auto &input = ecs.get_components<ecs::TextInputComponent>();
                for (auto &it: input) {
                    if (it.has_value()) {
                        packet.name = it->text;
                    }
                }
                if (!packet.name.length()) {
                    return;
                }
                packet.actionType = network::LobbyActionType::CreateRoom;
                ecs.run_event(packet);
                ecs::change_window(ecs, ecs::WindowType::LOBBY);
            },
            [buttonWidth](int screenWidth, int screenHeight) { return screenWidth / 2 - (buttonWidth / 2); },
            [buttonHeight](int screenWidth, int screenHeight) { return screenHeight / 3 + (buttonHeight * 2 + 100); }));
}

//
// Created by lferraro on 12/9/24.
//

#include "ecs/Systems.hpp"

void init_menu_entity (Registry &ecs) {
    auto JoinRoom = ecs.spawn_entity();

    int buttonWidth = 500;
    int buttonHeight = 100;

    ecs::TextComponent joinText("Join room", 54, 0, 0);
    ecs.add_component<ecs::ButtonComponent>(JoinRoom,ecs::ButtonComponent(
                                                    buttonWidth,
                                                    buttonHeight,
                                                    joinText,
                                                    [&ecs]() {
                                                        ecs::change_window(ecs, ecs::WindowType::LOBBY);
                                                    },
                                                    [buttonWidth](int screenWidth, int screenHeight) { return screenWidth / 2 - (buttonWidth / 2); },
                                                    [](int screenWidth, int screenHeight) { return screenHeight / 2; }
                                            )
    );

    auto CreateRoom = ecs.spawn_entity();

    ecs::TextComponent createText("Create room", 54, 0, 0);
    ecs.add_component<ecs::ButtonComponent>(CreateRoom,ecs::ButtonComponent(
                                                    buttonWidth,
                                                    buttonHeight,
                                                    createText,
                                                    [&ecs]() {
                                                        ecs::change_window(ecs, ecs::WindowType::LOBBY);
                                                    },
                                                    [buttonWidth](int screenWidth, int screenHeight) { return screenWidth / 2 - (buttonWidth / 2); },
                                                    [buttonHeight](int screenWidth, int screenHeight) { return screenHeight / 2 + (buttonHeight + 50); }
                                            )
    );
}

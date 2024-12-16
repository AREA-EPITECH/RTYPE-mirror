//
// Created by lferraro on 12/5/24.
//

#include "ecs/Systems.hpp"

void init_lobby_entity (Registry &ecs) {

    auto idText = ecs.spawn_entity();
    int id = 468682618;
    std::string idStr = "ID : " + std::to_string(id);
    ecs.add_component<ecs::TextComponent>(idText, {idStr, 54, 50, 50});

    std::vector<std::string> players_name = {"player1", "player2", "player3", "player4", "player5"};
    for (const auto & i : players_name) {
        auto playerId = ecs.spawn_entity();

        ecs.add_component<ecs::TextComponent>(playerId, {i, 54, 50, 50, 1});
    }

    auto readyButton = ecs.spawn_entity();

    int buttonWidth = 500;
    int buttonHeight = 100;

    ecs::TextComponent readyText("Ready", 54, 0, 0);
    ecs.add_component<ecs::ButtonComponent>(readyButton,ecs::ButtonComponent(
                                               buttonWidth,
                                               buttonHeight,
                                               readyText,
                                               [&ecs]() {
                                                   ecs::change_window(ecs, ecs::WindowType::GAME);
                                               },
                                               [buttonWidth](int screenWidth, int screenHeight) { return screenWidth * 0.66 - (buttonWidth / 2) + 20; },
                                               [](int screenWidth, int screenHeight) { return screenHeight - 150; }
                                           )
    );

}

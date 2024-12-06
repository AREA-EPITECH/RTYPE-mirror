//
// Created by lferraro on 12/5/24.
//

#include "ecs/Systems.hpp"

void init_lobby_entity (Registry &ecs) {

    const int screenWidth = GetScreenWidth();
    const int screenHeight = GetScreenHeight();

    auto idText = ecs.spawn_entity();
    int id = 468682618;
    std::string idStr = "ID : " + std::to_string(id);
    ecs.add_component<ecs::TextComponent>(idText, {idStr, 54, 50, 50});

    std::vector<std::string> players_name = {"player1", "player2", "player3", "player4", "player5"};
    for (int i = 0; i < players_name.size(); i++) {
        auto playerId = ecs.spawn_entity();

        ecs.add_component<ecs::TextComponent>(playerId, {players_name[i], 54, 50, 50, 1});
    }

    auto readyButton = ecs.spawn_entity();

    int buttonWidth = 500;
    int buttonHeight = 100;

    int buttonX = screenWidth * 0.66 - (buttonWidth / 2) + 20;
    int buttonY = screenHeight - 150;

    ecs::TextComponent readyText("Ready", 54, 0, 0);
    ecs.add_component<ecs::ButtonComponent>(readyButton,ecs::ButtonComponent(
                                               buttonWidth,
                                               buttonHeight,
                                               readyText,
                                               []() {
                                                   std::cout << "Player ready" << std::endl;
                                               },
                                               buttonX,
                                               buttonY
                                           )
    );

}

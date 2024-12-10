//
// Created by lferraro on 12/9/24.
//

#include "ecs/Systems.hpp"

void update_menu_selectors(Registry &ecs, int screenWidth, int screenHeight) {
    auto &buttons = ecs.get_components<ecs::ButtonComponent>();

    for (auto & button : buttons) {
        if (button.has_value()) {
            button.value().updateButton(screenWidth, screenHeight);
        }
    }
}

void display_menu_selectors(Registry &ecs) {
    auto &buttons = ecs.get_components<ecs::ButtonComponent>();

    for (auto & i : buttons) {
        if (i.has_value()) {
            auto &button = i.value();
            button.drawButton();
        }
    }
}

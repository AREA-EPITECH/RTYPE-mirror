//
// Created by lferraro on 12/9/24.
//

#include "ecs/Systems.hpp"

void update_menu_selectors(Registry &ecs, int screenWidth, int screenHeight) {
    auto &buttons = ecs.get_components<ecs::ButtonComponent>();

    for (int i = 0; i < buttons.size(); ++i) {
        if (buttons[i].has_value()) {
            buttons[i].value().updateButton(screenWidth, screenHeight);
        }
    }
}

void display_menu_selectors(Registry &ecs) {
    auto &buttons = ecs.get_components<ecs::ButtonComponent>();

    for (int i = 0; i < buttons.size(); i++) {
        if (buttons[i].has_value()) {
            auto &button = buttons[i].value();
            button.drawButton();
        }
    }
}

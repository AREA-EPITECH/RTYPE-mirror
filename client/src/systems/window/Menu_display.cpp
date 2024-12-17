/*
** EPITECH PROJECT, 2024
** r-type
** File description:
** Menu_display
*/

#include "ecs/Systems.hpp"

void update_menu_selectors(Registry &ecs, int screenWidth, int screenHeight) {
    auto &buttons = ecs.get_components<ecs::ButtonComponent>();

    for (auto & button : buttons) {
        if (button.has_value()) {
            button.value().updateButton(screenWidth, screenHeight);
        }
    }

    auto &boxs = ecs.get_components<ecs::ShowBoxComponent>();

    for (auto& box : boxs) {
        if (box.has_value()) {
            box.value().updateBox(screenWidth, screenHeight);
        }
    }
}

void display_menu_selectors(Registry &ecs) {
    auto &buttons = ecs.get_components<ecs::ButtonComponent>();
    auto &inputs = ecs.get_components<ecs::TextInputComponent>();
    auto &showbox = ecs.get_components<ecs::ShowBoxComponent>();
    ecs::WindowFocus focus = ecs::get_focus(ecs);

    for (auto & i : buttons) {
        if (i.has_value()) {
            auto &button = i.value();
            button.drawButton(focus);
        }
    }

    for (auto & input : inputs) {
        if (input.has_value()) {
            input.value().drawTextInput(GetScreenWidth(), GetScreenHeight());
            input.value().handleInput();
        }
    }

    for (auto & box : showbox) {
        if (box.has_value()) {
            if (!box.value().draw(focus)) {
                ecs.run_event(ecs::ChangeFocusEvent{ecs::MENU_FOCUS});
            }
        }
    }
}

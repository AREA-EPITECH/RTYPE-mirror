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

    auto& textInputs = ecs.get_components<ecs::TextInputComponent>();

    for (auto& textInput : textInputs) {
        if (textInput.has_value()) {
            textInput.value().updateInput(screenWidth, screenHeight);
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

    for (auto & i : buttons) {
        if (i.has_value()) {
            auto &button = i.value();
            button.drawButton();
        }
    }
    for (auto & input : inputs) {
        if (input.has_value()) {
            input.value().drawTextInput();
            input.value().handleInput();
        }
    }

    for (auto & box : showbox) {
        if (box.has_value()) {
            box.value().draw();
        }
    }

}

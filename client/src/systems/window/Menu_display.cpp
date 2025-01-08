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
    auto &images = ecs.get_components<ecs::ImageComponent>();
    auto &settings = ecs.get_components<ecs::SettingsComponent>();
    ecs::WindowFocus focus = ecs::get_focus(ecs);

    int s_width = GetScreenWidth();
    int s_height = GetScreenHeight();

    for (auto & i : buttons) {
        if (i.has_value()) {
            auto &button = i.value();
            button.drawButton(focus);
        }
    }

    for (auto & input : inputs) {
        if (input.has_value()) {
            input.value().drawTextInput(s_width, s_height, focus);
            input.value().handleInput();
        }
    }

    for (auto & box : showbox) {
        if (box.has_value()) {
            box.value().draw(focus);
        }
    }

    for (auto & image : images) {
        if (image.has_value()) {
            image.value().draw(s_width, s_height);
        }
    }

    for (auto & setting : settings) {
        if (setting.has_value()) {
            setting.value().background.width = (float)s_width;
            setting.value().background.height = (float)s_height;
            //setting.value().drawSettings(s_width, s_height);
            DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.9f));
            ecs.run_event(ecs::DisplaySettingEvent{});
        }
    }
}

/*
** EPITECH PROJECT, 2024
** r-type
** File description:
** Lobby_display
*/

#include "ecs/Systems.hpp"

void update_board_component(Registry &ecs, int screenWidth, int screenHeight) {

    auto &texts = ecs.get_components<ecs::TextComponent>();
    auto &buttons = ecs.get_components<ecs::ButtonComponent>();
    int players_ids = 0;

    for (int i = 0; i < texts.size(); i++) {
        if (texts[i].has_value()) {
            auto &text = texts[i].value();
            text.updateText(screenWidth, screenHeight);

            if (text.type == 1) {
                int lineY = 50 + text.fontSize + 50;
                text.posY = lineY + 50 + (100 * players_ids);
                players_ids++;
            }
        }
    }
    for (int i = 0; i < buttons.size(); ++i) {
        if (buttons[i].has_value()) {
            buttons[i].value().updateButton(screenWidth, screenHeight);
        }
    }

}

void display_board(Registry &ecs, int screenWidth, int screenHeight) {
    Color blackTransparent = {0, 48, 73, 200};
    Color redColor = {120, 0, 0, 255};
    DrawRectangle(0, 0, screenWidth / 3, screenHeight, blackTransparent);

    int fontSize = (int)((float)screenHeight * 0.05f);
    int textY = 50;

    auto &texts = ecs.get_components<ecs::TextComponent>();

    for (int i = 0; i < texts.size(); i++) {
        if (texts[i].has_value()) {
            auto &text_val = texts[i].value();
            DrawText(text_val.text.c_str(), text_val.posX, text_val.posY, text_val.fontSize, text_val.color);
        }
    }

    int lineStartX = 0;
    int lineWidth = screenWidth / 3;
    int lineY = textY + fontSize + 50;
    int lineThickness = 5;

    DrawRectangle(lineStartX, lineY, lineWidth, lineThickness, redColor);

    auto &buttons = ecs.get_components<ecs::ButtonComponent>();

    for (int i = 0; i < buttons.size(); i++) {
        if (buttons[i].has_value()) {
            auto &button = buttons[i].value();
            button.drawButton(ecs::get_focus(ecs));
        }
    }

    auto &vessels = ecs.get_components<ecs::VesselsComponent>();

    for (int i = 0; i < vessels.size(); ++i) {
        if (vessels[i].has_value()) {
            auto &vessel = vessels[i].value();
            if (vessel.drawable) {
                vessel.name.updateText(screenWidth, screenHeight);
                int textWidth = MeasureText(vessel.name.text.c_str(), vessel.name.fontSize);
                int posX = screenWidth * 0.66 - (textWidth / 2) + 20;
                vessel.name.posX = posX;
                DrawText(vessel.name.text.c_str(), vessel.name.posX, vessel.name.posY, vessel.name.fontSize, vessel.name.color);
            }
        }
    }
}

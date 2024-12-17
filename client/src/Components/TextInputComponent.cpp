/*
** EPITECH PROJECT, 2024
** r-type
** File description:
** TextInputComponent
*/

#include <utility>

#include "ecs/Components.hpp"

namespace ecs {
    TextInputComponent::TextInputComponent(Rectangle _inputBox, std::string  _defaultText, size_t _maxLength,
                                                Color _boxColor, Color _textColor, Color _borderColor,
                                                std::function<int(int screenWidth, int screenHeight)> _dynamicX,
                                                std::function<int(int screenWidth, int screenHeight)> _dynamicY)
            : inputBox(_inputBox), placeholder(std::move(_defaultText)), boxColor(_boxColor),
    textColor(_textColor), borderColor(_borderColor), isFocused(false), maxLength(_maxLength),
    dynamicX(std::move(_dynamicX)), dynamicY(std::move(_dynamicY)) {}

    void TextInputComponent::drawTextInput() {
        if (dynamicX) inputBox.x = (float)dynamicX(GetScreenWidth(), GetScreenHeight());
        if (dynamicY) inputBox.y = (float)dynamicY(GetScreenWidth(), GetScreenHeight());

        int fontSize = 20;
        GuiSetStyle(DEFAULT, TEXT_SIZE, fontSize);
        GuiSetStyle(DEFAULT, BORDER_COLOR_NORMAL, ColorToInt(borderColor));
        GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, ColorToInt(BLACK));

        char buffer[512] = {0};
        strncpy(buffer, text.c_str(), maxLength);

        bool active = GuiTextBox(inputBox, buffer, maxLength, isFocused);

        text = std::string(buffer);

        if (active) {
            isFocused = true;
        }

        if (!isFocused && text.empty()) {
            Vector2 textSize = MeasureTextEx(GetFontDefault(), placeholder.c_str(), (float)fontSize, 1);
            float textY = inputBox.y + (inputBox.height - textSize.y) / 2;

            DrawText(placeholder.c_str(), (int)inputBox.x + 20, (int)textY, fontSize, GRAY);
        }
    }

    void TextInputComponent::handleInput() {
        Vector2 mousePosition = GetMousePosition();

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            isFocused = CheckCollisionPointRec(mousePosition, inputBox);
        }
    }


}
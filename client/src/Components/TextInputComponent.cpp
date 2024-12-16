/*
** EPITECH PROJECT, 2024
** r-type
** File description:
** TextInputComponent
*/

#include "ecs/Components.hpp"

namespace ecs {
    TextInputComponent::TextInputComponent(Rectangle _inputBox, const std::string& _defaultText, size_t _maxLength,
                                                Color _boxColor, Color _textColor, Color _borderColor,
                                                std::function<int(int screenWidth, int screenHeight)> _dynamicX,
                                                std::function<int(int screenWidth, int screenHeight)> _dynamicY)
            : inputBox(_inputBox), placeholder(_defaultText), boxColor(_boxColor),
              textColor(_textColor), borderColor(_borderColor), isFocused(false),
              maxLength(_maxLength), dynamicX(_dynamicX), dynamicY(_dynamicY) {}

    void TextInputComponent::drawTextInput() {
        Color currentBoxColor = isFocused ? Fade(boxColor, 0.8f) : boxColor;

        DrawRectangleRec(inputBox, currentBoxColor);

        DrawRectangleLinesEx(inputBox, 2, borderColor);

        int textWidth = MeasureText(text.c_str(), 20);
        float textX = inputBox.x + 50;
        float textY = inputBox.y + (inputBox.height - 20) / 2;
        DrawText(text.c_str(), textX, textY, 20, textColor);

        if (isFocused && (GetTime() * 2) - int(GetTime() * 2) < 0.5) {
            DrawRectangle(textX + textWidth + 2, textY, 2, 20, textColor);
        }
    }

    void TextInputComponent::handleInput() {
        Vector2 mousePosition = GetMousePosition();

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            isFocused = CheckCollisionPointRec(mousePosition, inputBox);
            if (isFocused && text == placeholder)
                text = "";
        }

        if (isFocused) {
            int key = GetKeyPressed();
            while (key > 0) {
                if ((key >= 32) && (key <= 125)) {
                    if (text.size() < maxLength) {
                        text += static_cast<char>(key);
                    }
                } else if (key == KEY_BACKSPACE && !text.empty()) {
                    text.pop_back();
                }
                key = GetKeyPressed();
            }
        } else {
            if (text.empty()) {
                text = placeholder;
            }
        }
    }

    void TextInputComponent::updateInput(int screenWidth, int screenHeight) {
        if (dynamicX) {
            inputBox.x = dynamicX(screenWidth, screenHeight);
        }
        if (dynamicY) {
            inputBox.y = dynamicY(screenWidth, screenHeight);
        }
    }
}
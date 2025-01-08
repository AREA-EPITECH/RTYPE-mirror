/*
** EPITECH PROJECT, 2024
** r-type
** File description:
** TextInputComponent
*/

#include <utility>

#include "ecs/Components.hpp"

namespace ecs {
    NumberInputComponent::NumberInputComponent(Rectangle _inputBox, WindowFocus _focus, std::string  _defaultText, int _minValue, int _maxValue,
                                                Color _boxColor, Color _textColor, Color _borderColor,
                                                std::function<int(int screenWidth, int screenHeight)> _dynamicX,
                                                std::function<int(int screenWidth, int screenHeight)> _dynamicY)
            : inputBox(_inputBox), focus(_focus), placeholder(std::move(_defaultText)), boxColor(_boxColor),
    textColor(_textColor), borderColor(_borderColor), isFocused(false), minValue(_minValue), maxValue(_maxValue),
    dynamicX(std::move(_dynamicX)), dynamicY(std::move(_dynamicY)) {}

    void NumberInputComponent::drawNumberInput(int screenWidth, int screenHeight, WindowFocus _focus) {
        if (dynamicX) inputBox.x = (float)dynamicX(screenWidth, screenHeight);
        if (dynamicY) inputBox.y = (float)dynamicY(screenWidth, screenHeight);

        int previousState = GuiGetState();

        if (focus != _focus) {
            GuiDisable();
        }

        int fontSize = 20;
        GuiSetStyle(DEFAULT, TEXT_SIZE, fontSize);
        GuiSetStyle(DEFAULT, BORDER_COLOR_NORMAL, ColorToInt(borderColor));
        GuiSetStyle(DEFAULT, BASE_COLOR_NORMAL, ColorToInt(Color{193, 18, 31, 200})); // Couleur de fond
        GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, ColorToInt(BLACK));

        bool active = GuiValueBox(inputBox, "", &value, minValue, maxValue, isFocused);

        if (focus == _focus) {
            if (active) {
                isFocused = true;
            }
        }

        if (!isFocused) {
            Vector2 textSize = MeasureTextEx(GetFontDefault(), placeholder.c_str(), (float)fontSize, 1);
            float textY = inputBox.y + (inputBox.height - textSize.y) / 2;
        }

        GuiSetState(previousState);
    }

    void NumberInputComponent::handleInput() {

        Vector2 mousePosition = GetMousePosition();

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            isFocused = CheckCollisionPointRec(mousePosition, inputBox);
        }
    }


}
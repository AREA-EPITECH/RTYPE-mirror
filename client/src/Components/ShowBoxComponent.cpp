/*
** EPITECH PROJECT, 2024
** r-type
** File description:
** ShowBoxComponent
*/

#include <utility>

#include "ecs/Components.hpp"

namespace ecs {

    ShowBoxComponent::ShowBoxComponent(Rectangle _boxRect, std::string _message, Color _boxColor, Color _textColor,
                                       std::string _textInput, std::string _closeButtonText, std::string _continueButtonText,
                                       std::function<int(int screenWidth, int screenHeight)> _dynamicX,
                                       std::function<int(int screenWidth, int screenHeight)> _dynamicY)
        : boxRect(_boxRect), message(std::move(_message)), boxColor(_boxColor), textColor(_textColor),
          isVisible(false), dynamicX(std::move(_dynamicX)), dynamicY(std::move(_dynamicY)),
          closeButtonText(std::move(_closeButtonText)),
          continueButtonText(std::move(_continueButtonText))
          {
            textInput.placeholder = std::move(_textInput);

            textInput.maxLength = 20;
            if (dynamicX) boxRect.x = dynamicX(GetScreenWidth(), GetScreenHeight());
            if (dynamicY) boxRect.y = dynamicY(GetScreenWidth(), GetScreenHeight());

            textInput.dynamicX = [](int screenWidth, int screenHeight) {
                return static_cast<int>(screenWidth + 10);
            };
            textInput.dynamicY = [](int screenWidth, int screenHeight) {
                return static_cast<int>(screenHeight + 40);
            };

            textInput.inputBox = {boxRect.x + 10, boxRect.y + 40, boxRect.width - 20, 30};
          }

    void ShowBoxComponent::draw() {
        if (!isVisible) return;

        if (dynamicX) boxRect.x = dynamicX(GetScreenWidth(), GetScreenHeight());
        if (dynamicY) boxRect.y = dynamicY(GetScreenWidth(), GetScreenHeight());

        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.5f));
        GuiPanel(boxRect, message.c_str());

        textInput.drawTextInput( boxRect.x,  boxRect.y);
        textInput.handleInput();

        Rectangle closeButtonRect = {boxRect.x + 10, boxRect.y + boxRect.height - 50, (boxRect.width - 30) / 2, 40};
        Rectangle continueButtonRect = {closeButtonRect.x + closeButtonRect.width + 10, closeButtonRect.y, closeButtonRect.width, closeButtonRect.height};

        if (GuiButton(closeButtonRect, closeButtonText.c_str())) {
            isVisible = false;
        }

        if (GuiButton(continueButtonRect, continueButtonText.c_str())) {
            isVisible = false;
        }
    }

    void ShowBoxComponent::handleClick(Vector2 mousePosition) {
    }

    void ShowBoxComponent::updateBox(int screenWidth, int screenHeight) {
        if (dynamicX) boxRect.x = dynamicX(screenWidth, screenHeight);
        if (dynamicY) boxRect.y = dynamicY(screenWidth, screenHeight);

        textInput.inputBox = {boxRect.x + 10, boxRect.y + 40, boxRect.width - 20, 30};
    }
}

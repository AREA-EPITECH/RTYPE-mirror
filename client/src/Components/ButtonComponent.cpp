/*
** EPITECH PROJECT, 2024
** r-type
** File description:
** ButtonComponent
*/

#include "ecs/Components.hpp"

namespace ecs {
    ButtonComponent::ButtonComponent(int _buttonWidth, int _buttonHeight, ecs::TextComponent _text,
                                          std::function<void()> _onClick, std::function<int(int, int)> _dynamicX,
                                          std::function<int(int, int)> _dynamicY, Color _color) :
            buttonWidth(_buttonWidth),
            buttonHeight(_buttonHeight),
            color(_color),
            text(std::move(_text)),
            onClick(std::move(_onClick)),
            dynamicX(std::move(_dynamicX)),
            dynamicY(std::move(_dynamicY))
    {
        if (dynamicX) buttonX = dynamicX(GetScreenWidth(), GetScreenHeight());
        if (dynamicY) buttonY = dynamicY(GetScreenWidth(), GetScreenHeight());

        int textWidth = MeasureText(text.text.c_str(), text.fontSize);
        text.posX = buttonX + (buttonWidth - textWidth) / 2;
        text.posY = buttonY + (buttonHeight - text.fontSize) / 2;
    }

    void ButtonComponent::updateButton(int screenWidth, int screenHeight) {
        if (dynamicX) buttonX = dynamicX(screenWidth, screenHeight);
        if (dynamicY) buttonY = dynamicY(screenWidth, screenHeight);

        int textWidth = MeasureText(text.text.c_str(), text.fontSize);
        text.posX = buttonX + (buttonWidth - textWidth) / 2;
        text.posY = buttonY + (buttonHeight - text.fontSize) / 2;
    }

    bool ButtonComponent::isButtonPressed(Vector2 mousePosition) {
        if (mousePosition.x >= buttonX && mousePosition.x <= (buttonX + buttonWidth) &&
            mousePosition.y >= buttonY && mousePosition.y <= (buttonY + buttonHeight)) {
            if (onClick) onClick();
            return true;
        }
        return false;
    }

    void ButtonComponent::drawButton() const {
        Vector2 mousePosition = GetMousePosition();

        Color buttonColor = color;
        Color textColor = text.color;
        if (isHover(mousePosition)) {
            buttonColor = text.color;
            textColor = color;
        }

        DrawRectangle(buttonX, buttonY, buttonWidth, buttonHeight, buttonColor);

        int textX = buttonX + (buttonWidth - MeasureText(text.text.c_str(), text.fontSize)) / 2;
        int textY = buttonY + (buttonHeight - text.fontSize) / 2;
        DrawText(text.text.c_str(), textX, textY, text.fontSize, textColor);
    }

    bool ButtonComponent::isHover(Vector2 mousePosition) const {
        return mousePosition.x >= buttonX &&
               mousePosition.x <= buttonX + buttonWidth &&
               mousePosition.y >= buttonY &&
               mousePosition.y <= buttonY + buttonHeight;
    }

}
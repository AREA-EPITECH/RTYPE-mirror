//
// Created by lferraro on 12/9/24.
//

#include "ecs/Components.hpp"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

ecs::ButtonComponent::ButtonComponent(int _buttonWidth, int _buttonHeight, ecs::TextComponent _text,
                                      std::function<void()> _onClick, std::function<int(int, int)> _dynamicX,
                                      std::function<int(int, int)> _dynamicY, Color _color) :
                                      buttonWidth(_buttonWidth),
                                      buttonHeight(_buttonHeight),
                                      color(_color),
                                      text(std::move(_text)),
                                      onClick(std::move(_onClick)),
                                      dynamicX(std::move(_dynamicX)),
                                      dynamicY(std::move(_dynamicY))  {


        if (dynamicX) buttonX = dynamicX(GetScreenWidth(), GetScreenHeight());
        if (dynamicY) buttonY = dynamicY(GetScreenWidth(), GetScreenHeight());

        int textWidth = MeasureText(text.text.c_str(), text.fontSize);
        text.posX = buttonX + (buttonWidth - textWidth) / 2;
        text.posY = buttonY + (buttonHeight - text.fontSize) / 2;
}

void ecs::ButtonComponent::drawButton() const {
    Rectangle buttonRect = {static_cast<float>(buttonX), static_cast<float>(buttonY),
                            static_cast<float>(buttonWidth), static_cast<float>(buttonHeight)};
    if (GuiButton(buttonRect, text.text.c_str()) && onClick) {
        onClick();
    }
}

void ecs::ButtonComponent::updateButton(int screenWidth, int screenHeight) {
    if (dynamicX) buttonX = dynamicX(screenWidth, screenHeight);
    if (dynamicY) buttonY = dynamicY(screenWidth, screenHeight);

    int textWidth = MeasureText(text.text.c_str(), text.fontSize);
    text.posX = buttonX + (buttonWidth - textWidth) / 2;
    text.posY = buttonY + (buttonHeight - text.fontSize) / 2;
}

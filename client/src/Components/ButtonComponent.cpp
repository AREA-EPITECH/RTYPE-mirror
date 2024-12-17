/*
** EPITECH PROJECT, 2024
** r-type
** File description:
** ButtonComponent
*/

#include "ecs/Components.hpp"

namespace ecs {
    ButtonComponent::ButtonComponent(int _buttonWidth, int _buttonHeight, std::string _text,
                    WindowFocus _focus,
                    std::function<void()> _onClick,
                    std::function<int(int, int)> _dynamicX,
                    std::function<int(int, int)> _dynamicY,
                    Color _buttonColor)
            : buttonWidth(_buttonWidth),
              buttonHeight(_buttonHeight),
              text(std::move(_text)),
              onClick(std::move(_onClick)),
              dynamicX(std::move(_dynamicX)),
              dynamicY(std::move(_dynamicY)),
              focus(_focus),
              buttonColor(_buttonColor)
    {
        updateButton(GetScreenWidth(), GetScreenHeight());
    }

    void ButtonComponent::updateButton(int screenWidth, int screenHeight) {
        if (dynamicX) buttonX = dynamicX(screenWidth, screenHeight);
        if (dynamicY) buttonY = dynamicY(screenWidth, screenHeight);
    }

    void ButtonComponent::drawButton(WindowFocus _focus) {
        int previousState = GuiGetState();

        if (focus != _focus) {
            GuiDisable();
        }
        GuiSetStyle(DEFAULT, BASE_COLOR_NORMAL, ColorToInt(Color{193, 18, 31, 200}));
        GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, ColorToInt(BLACK));
        if (GuiButton({static_cast<float>(buttonX), static_cast<float>(buttonY),
                       static_cast<float>(buttonWidth), static_cast<float>(buttonHeight)},
                      text.c_str())) {
            if (onClick && focus == _focus) {
                onClick();
            }
        }

        GuiSetState(previousState);
    }

}

/*
** EPITECH PROJECT, 2024
** r-type
** File description:
** ButtonComponent
*/

#include "ecs/Components.hpp"

namespace ecs
{
    ButtonComponent::ButtonComponent(int _buttonWidth, int _buttonHeight, std::string _text, WindowFocus _focus,
                                     std::function<void()> _onClick, std::function<int(int, int)> _dynamicX,
                                     std::function<int(int, int)> _dynamicY, int _text_size, int _border_width,
                                     Color _base_color_normal, Color _text_color_normal, Color _border_color_normal,
                                     Color _base_color_focused, Color _text_color_focused,
                                     Color _border_color_focused) :
        buttonWidth(_buttonWidth), buttonHeight(_buttonHeight), text(std::move(_text)), onClick(std::move(_onClick)),
        dynamicX(std::move(_dynamicX)), dynamicY(std::move(_dynamicY)), focus(_focus), text_size(_text_size),
        border_width(_border_width), base_color_normal(_base_color_normal), text_color_normal(_text_color_normal),
        border_color_normal(_border_color_normal), base_color_focused(_base_color_focused),
        text_color_focused(_text_color_focused), border_color_focused(_border_color_focused)
    {
        updateButton(GetScreenWidth(), GetScreenHeight());
    }

    void ButtonComponent::updateButton(const int screenWidth, const int screenHeight)
    {
        if (dynamicX)
            buttonX = dynamicX(screenWidth, screenHeight);
        if (dynamicY)
            buttonY = dynamicY(screenWidth, screenHeight);
    }

    void ButtonComponent::drawButton(const WindowFocus _focus) const
    {
        const int previousState = GuiGetState();

        if (focus != _focus)
        {
            GuiDisable();
        }

        GuiLoadStyleDefault();
        GuiSetStyle(DEFAULT, TEXT_SIZE, text_size);
        GuiSetStyle(DEFAULT, BORDER_WIDTH, border_width);

        // Normal
        GuiSetStyle(DEFAULT, BASE_COLOR_NORMAL, ColorToInt(base_color_normal));
        GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, ColorToInt(text_color_normal));
        GuiSetStyle(DEFAULT, BORDER_COLOR_NORMAL, ColorToInt(border_color_normal));

        // Focused & pressed
        GuiSetStyle(DEFAULT, BASE_COLOR_FOCUSED, ColorToInt(base_color_focused));
        GuiSetStyle(DEFAULT, TEXT_COLOR_FOCUSED, ColorToInt(text_color_focused));
        GuiSetStyle(DEFAULT, BORDER_COLOR_FOCUSED, ColorToInt(border_color_focused));
        GuiSetStyle(DEFAULT, BASE_COLOR_PRESSED, ColorToInt(base_color_focused));
        GuiSetStyle(DEFAULT, TEXT_COLOR_PRESSED, ColorToInt(text_color_focused));
        GuiSetStyle(DEFAULT, BORDER_COLOR_PRESSED, ColorToInt(border_color_focused));

        if (GuiButton({static_cast<float>(buttonX), static_cast<float>(buttonY), static_cast<float>(buttonWidth),
                       static_cast<float>(buttonHeight)},
                      text.c_str()))
        {
            if (onClick && focus == _focus)
            {
                onClick();
            }
        }

        GuiSetState(previousState);
    }

} // namespace ecs
